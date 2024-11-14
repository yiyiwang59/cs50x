import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute(
        "SELECT symbol, SUM(num_shares) AS shares FROM transaction_history WHERE user_id = ? AND symbol != 'BANK' GROUP BY symbol", session['user_id'])
    user_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
    grand_total = user_balance
    for stock in stocks:
        current_price = lookup(stock['symbol'])['price']
        stock['quote'] = current_price
        stock['total_value'] = current_price * int(stock['shares'])
        grand_total += stock['total_value']
    return render_template("index.html", stock_data=stocks, grand_value=grand_total, balance=user_balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        num_shares = request.form.get("shares")
        try:
            num_shares = int(num_shares)
        except ValueError:
            return apology("Invalid shares", 400)
        else:
            if symbol and num_shares and num_shares > 1:
                stock_quote = lookup(symbol)
                if stock_quote is not None:
                    current_balance = db.execute(
                        "SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
                    cost = stock_quote['price'] * num_shares
                    if cost < current_balance:
                        updated_balance = current_balance - cost
                        db.execute("INSERT INTO transaction_history (user_id, timestamp, price, balance, num_shares, symbol, type) VALUES (?, ?, ?, ?, ?, ?, 'buy')",
                                   session["user_id"], datetime.datetime.now(), stock_quote['price'], updated_balance, num_shares, symbol)
                        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                                   updated_balance, session["user_id"])
                        return redirect("/")
                    else:
                        return apology("Not enough cash", 400)
                else:
                    return apology("Invalid Symbol", 400)
            else:
                return apology("Must provide Symbol and Shares", 400)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    data = db.execute(
        "SELECT timestamp, symbol, num_shares, ROUND(price, 2) AS price, ROUND(balance, 2) AS balance, type FROM transaction_history WHERE user_id = ? ORDER BY timestamp DESC", session['user_id'])
    return render_template("history.html", history=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote_data = lookup(symbol)
        if quote_data is not None:
            quote_data['price'] = float(quote_data['price'])
            return render_template("quoted.html", quote=quote_data)
        else:
            return apology("Invalid Symbol", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if username and password and confirmation:
            if password == confirmation:
                try:
                    db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                               username, generate_password_hash(password))
                except ValueError:
                    return apology("username already exists", 400)
                else:
                    return redirect("/login")
            else:
                return apology("password confirmation does not match.", 400)
        else:
            return apology("Missing username/password", 400)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    stocks = db.execute(
        "SELECT symbol, SUM(num_shares) AS shares FROM transaction_history WHERE user_id = ? GROUP BY symbol", session['user_id'])
    stock_options = [stock['symbol'] for stock in stocks]
    if request.method == "POST":
        stock = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        available_shares = int(db.execute(
            "SELECT SUM(num_shares) AS shares FROM transaction_history WHERE user_id = ? AND symbol = ?", session['user_id'], stock)[0]['shares'])
        if stock and shares:
            if shares < available_shares:
                current_balance = db.execute(
                    "SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
                quote = lookup(stock)['price']
                updated_balance = current_balance + quote * shares
                db.execute("INSERT INTO transaction_history (user_id, timestamp, price, balance, num_shares, symbol, type) VALUES (?, ?, ?, ?, ?, ?, 'sell')",
                           session["user_id"], datetime.datetime.now(), quote, updated_balance, -int(shares), stock)
                db.execute("UPDATE users SET cash = ? WHERE id = ?",
                           updated_balance, session["user_id"])
                return redirect("/")
            else:
                return apology("Not enough shares available", 400)
        else:
            return apology("Please provide stock and number of shares to sell", 400)
    else:
        return render_template("sell.html", options=stock_options)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit Cash"""
    if request.method == "POST":
        amount = request.form.get("amount")
        try:
            amount = float(amount)
        except TypeError:
            return apology("Invalid input", 403)
        else:
            current_balance = db.execute(
                "SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
            updated_balance = current_balance + amount
            db.execute("INSERT INTO transaction_history (user_id, timestamp, price, balance, symbol, type) VALUES (?, ?, ?, ?, 'BANK', 'deposit')",
                       session["user_id"], datetime.datetime.now(), amount, updated_balance)
            db.execute("UPDATE users SET cash = ? WHERE id = ?",
                       updated_balance, session["user_id"])
            return redirect("/")
    else:
        return render_template("deposit.html")
