SELECT name FROM people WHERE id in (
    SELECT person_id FROM directors WHERE movie_id in (
        SELECT id FROM movies WHERE id in (
            SELECT movie_id FROM ratings WHERE rating >= 9.0)));
