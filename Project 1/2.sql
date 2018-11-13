select star
from appeared_in
where star != 'brad pitt' and movie in 
(select movie
from appeared_in
where star = 'brad pitt');
