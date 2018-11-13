create view v1 as
select a.star as astar, c.star as bstar, a.couple_num as cnumv
from in_couple a, married b, in_couple c, married d
where a.couple_num = b.couple_num and c.couple_num = d.couple_num and b.couple_num = d.couple_num
and a.star != c.star
order by a.couple_num;

create view v2 as
select a.star astar2, b.star as bstar2, a.movie as mov
from appeared_in a, appeared_in b
where a.movie = b.movie and a.star != b.star;

create view v3 as
select b.couple_num as cnum3
from made_money a, married b
where a.day_opened > b.day
order by b.couple_num;

select v1.astar, v2.bstar2, v2.mov
from v1, v2, v3
where v1.astar = v2.astar2 and v1.bstar = v2.bstar2 and v1.cnum = v3.cnum3
group by v1.astar
having v1.astar < v2.bstar2
