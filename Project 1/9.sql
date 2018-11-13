select c.star
from in_couple c, divorced d
where c.couple_num = d.couple_num
group by c.star
having count(*) >= 2