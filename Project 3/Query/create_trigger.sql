drop trigger if exists q_1;
drop trigger if exists q_2;

DELIMITER $$

create trigger q_1
before insert on uosoffering
for each row
begin
if new.enrollment < new.maxenrollment / 2 
then
	SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Enrollment number of a course goes below 50% of the MaxEnrollment !!!!'; 
end if;
end;
$$

DELIMITER $$

create trigger q_2
before update on uosoffering
for each row
begin
if new.enrollment < new.maxenrollment / 2 
then
	SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Enrollment number of a course goes below 50% of the MaxEnrollment !!!!'; 
end if;
end;

$$
