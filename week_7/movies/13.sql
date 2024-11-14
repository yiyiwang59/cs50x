SELECT name FROM people WHERE id in (
    SELECT A.person_id FROM stars A, stars B WHERE (
        A.person_id <> B.person_id AND
        A.movie_id = B.movie_id AND
        B.person_id = (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958)
    )
);
