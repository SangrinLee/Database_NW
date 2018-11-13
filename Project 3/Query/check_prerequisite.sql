drop procedure if exists check_prerequisite;

DELIMITER $$

create procedure check_prerequisite (in id int, in course char(20))
begin
start transaction;

select d.uoscode
from transcript d
where d.studid = id and (d.grade = "D" or d.grade = NULL) and d.uoscode = 
(select e.PrereqUoSCode
from requires e
where e.UoSCode = course
);
commit;
end

$$
DELIMITER ;