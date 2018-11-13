select a.star, a.couple_num, c.star, count(a.star)
from in_couple a, married b, in_couple c
where a.couple_num = b.couple_num and  c.couple_num = b.couple_num and c.star != a.star
group by a.star, c.star
having count(a.star) > 1 and a.star < c.star