SELECT AVG(ratings.rating) FROM movies LEFT JOIN ratings ON movies.id = ratings.movie_id WHERE movies.year = 2012;
