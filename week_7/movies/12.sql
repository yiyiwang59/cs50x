SELECT title FROM movies WHERE id in (
    SELECT A.movie_id
    FROM stars A, stars B
    WHERE (
        A.person_id <> B.person_id AND
        A.movie_id = B.movie_id AND
        A.person_id in (SELECT id FROM people WHERE name = "Jennifer Lawrence" OR name = "Bradley Cooper") AND
        B.person_id in (SELECT id FROM people WHERE name = "Jennifer Lawrence" OR name = "Bradley Cooper")
    )
);
