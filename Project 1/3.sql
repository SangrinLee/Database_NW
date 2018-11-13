select sum(how_much)
from made_money c
where c.movie in
	( select a.movie
	from appeared_in a, appeared_in b
	where a.star = 'tom hanks' and b.star = 'rita wilson' and a.movie = b.movie);