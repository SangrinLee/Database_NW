drop procedure if exists check_quarter;

DELIMITER $$

create procedure check_quarter (in course char(20), in this_year int, in this_sem char(20), in next_year int, in next_sem char(20), in in_year int, in in_sem char(20))
begin
start transaction;

select * 
from uosoffering a
where a.uoscode = course and ((a.semester = this_sem and a.year = this_year and a.semester = in_sem and a.year = in_year) or (a.semester = next_sem and a.year = next_year and a.semester = in_sem and a.year = in_year));

commit;
end

$$
DELIMITER ;