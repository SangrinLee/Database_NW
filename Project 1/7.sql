select star
from appeared_in
group by star
having count(*) = (
select max(a.c)
from (select count(*) as c from appeared_in group by star) a);