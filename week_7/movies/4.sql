SELECT COUNT(*) FROM movies LEFT JOIN ratings ON movies.id = ratings.movie_id WHERE ratings.rating = 10.0;
