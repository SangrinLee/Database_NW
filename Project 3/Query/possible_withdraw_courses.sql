drop procedure if exists possible_withdraw_courses;

DELIMITER $$

create procedure possible_withdraw_courses (in in_id int, in this_year int, in this_sem char(20), in next_year int, in next_sem char(20))
begin

start transaction;
select *
from transcript
where studid = in_id and ((semester = this_sem and year = this_year) or (semester = next_sem and year = next_year)) and grade is NULL;

commit;
end

$$
DELIMITER ;
