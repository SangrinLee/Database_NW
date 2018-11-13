select b.star
from divorced a, in_couple b 
where b.star != 'ben affleck' and a.couple_num = b.couple_num and a.couple_num in
( select c.couple_num
from in_couple c
where c.star = 'ben affleck');