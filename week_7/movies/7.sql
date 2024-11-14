SELECT title, ratings.rating FROM movies LEFT JOIN ratings ON movies.id = ratings.movie_id WHERE ratings.rating NOT NULL AND year = 2010 ORDER BY rating DESC, title;
