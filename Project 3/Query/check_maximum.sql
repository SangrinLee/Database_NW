drop procedure if exists check_maximum;

DELIMITER $$

create procedure check_maximum (in course char(20), in in_year int, in in_semester char(20))
begin
start transaction;

select * from uosoffering
where uoscode = course and enrollment < maxenrollment
and year = in_year and semester = in_semester;

commit;
end

$$
DELIMITER ;