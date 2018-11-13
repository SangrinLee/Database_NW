drop procedure if exists course_add;

DELIMITER $$

create procedure course_add (in id int, in course char(20), in in_quarter char(20), in in_year int)
begin
start transaction;

insert into transcript
values (id, course, in_quarter, in_year, NULL);

update uosoffering
set enrollment = enrollment + 1
where uoscode = course and year = in_year and semester = in_quarter;

commit;
end

$$
DELIMITER ;