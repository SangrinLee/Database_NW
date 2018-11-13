drop procedure if exists check_prev_course;

DELIMITER $$

create procedure check_prev_course (in id int, in course char(20))
begin
start transaction;

select distinct a.uoscode
from unitofstudy a
where a.uoscode = course and a.uoscode in
(select uoscode
from transcript
where studid = id);

commit;

end

$$
DELIMITER ;