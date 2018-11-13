select *, avg(d.how_much)
from appeared_in c, made_money d
where c.movie = d.movie
group by c.star
having avg(d.how_much) = (
select max(newt.maxim)
from (select avg(b.how_much) as maxim from appeared_in a, made_money b where a.movie = b.movie group by star) newt);
