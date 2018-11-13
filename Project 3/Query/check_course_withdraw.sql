drop procedure if exists check_course_withdraw;

DELIMITER $$

create procedure check_course_withdraw (in id int, in course char(20), in in_quarter char(20), in in_year int)
begin

start transaction ;
select *
from transcript
where studid = id and uoscode = course and semester = in_quarter and year = in_year and grade is NULL;

commit;
end

$$
DELIMITER ;