select c.star
from married a, divorced b, in_couple c
where a.day = b.day and a.couple_num = c.couple_num
