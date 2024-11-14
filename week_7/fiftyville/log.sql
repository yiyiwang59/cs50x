-- Keep a log of any SQL queries you execute as you solve the mystery.
--Looking at initial crime scene reports on that date and location
SELECT *
FROM crime_scene_reports
WHERE
    month = 7 AND
    day = 28 AND
    year = 2023 AND
    street = "Humphrey Street";


-- information from the bakery:
-- looking at security logs at the bakery since 3 witnesses mentioned the bakery around 10:15am

SELECT * FROM interviews WHERE year = 2023 AND month = 7 AND day = 28;
--information from interviews:
-- check license plates leaving the bakery within 10 minutes of the time of theft (10:15am)
-- check phone logs for phone calls made with duration < 1 min on day of theft
-- check passengers to flights leaving fiftyville early the next day
-- check atm transactions withdrawals on day of theft at Leggett Street

--people who exited the bakery within 10 min after the theft
--CREATE VIEW bakery_suspects AS
SELECT NAME, id
FROM people
WHERE
    license_plate in (
        SELECT license_plate
        FROM bakery_security_logs
        WHERE
            year = 2023 AND
            month = 07 AND
            day = 28 AND
            hour = 10 AND
            minute >= 15 AND
            minute <= 25);

--people who made withdrawals on day of theft at the Leggett Street location
--CREATE VIEW bank_withdrawal_suspects AS
SELECT NAME, id
FROM people
WHERE id in (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number in (
        SELECT account_number
        FROM atm_transactions
        WHERE
            year = 2023 AND
            month = 07 AND
            day = 28 AND
            atm_location = "Leggett Street" AND
            transaction_type = "withdraw"));
-- Current suspects: Iman, Diana, Luca, Bruce

--people who made a call less than one minute on the day of the theft
--CREATE VIEW phone_call_suspects AS
SELECT NAME, id
FROM people
WHERE phone_number in (
    SELECT caller
    FROM phone_calls
    WHERE
        year = 2023 AND
        day = 28 AND
        month = 07 AND
        duration < 60);

--Current suspects: Bruce

-- people who were on a flight out put Fiftyville the next day
--CREATE VIEW passenger_suspects AS
SELECT NAME, id
FROM people
WHERE passport_number in (
    SELECT passport_number
    FROM passengers
    WHERE flight_id in (
        SELECT id
        FROM flights
        WHERE origin_airport_id = (
            SELECT id
            FROM airports
            WHERE city = "Fiftyville") AND
                year = 2023 AND
                month = 07 AND
                day = 29
            ORDER BY hour, minute
    )
);

--Final Suspect: Bruce

--Bruce's Flight: New York City
SELECT city
FROM airports
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE id = (
        SELECT flight_id
        FROM passengers
        WHERE passport_number = (
            SELECT passport_number
            FROM people
            WHERE NAME = "Bruce")));

-- Bruce's Accomplice: Robin
SELECT NAME
FROM people
WHERE phone_number = (
    SELECT receiver
    FROM phone_calls
    WHERE year = 2023 AND
    month = 07 AND
    day = 28 AND
    caller = (
        SELECT phone_number
        FROM people
        WHERE NAME = "Bruce"
        ));


-- SELECT NAME
-- FROM people WHERE id in (
--     SELECT bakery_suspects.id
--     FROM bakery_suspects
--     JOIN passenger_suspects
--     JOIN phone_call_suspects
--     JOIN bank_withdrawal_suspects
--     WHERE bakery_suspects.id = passenger_suspects.id = phone_call_suspects.id = bank_withdrawal_suspects.id);
