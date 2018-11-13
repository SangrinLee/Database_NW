drop procedure if exists course_withdraw;

DELIMITER $$

create procedure course_withdraw (in id int, in course char(20), in in_quarter char(20), in in_year int)
begin
start transaction;

delete from transcript
where studid = id and course = uoscode and semester = in_quarter and year = in_year;

update uosoffering
set enrollment = enrollment - 1
where uoscode = course and year = in_year and semester = in_quarter;

commit;
end

$$
DELIMITER ;