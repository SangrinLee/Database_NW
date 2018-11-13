drop procedure if exists check_maximum_preferred;

DELIMITER $$

create procedure check_maximum_preferred (in course char(20), in in_year int, in in_semester char(20), in maximum int, in classtype char(20))
begin
start transaction;

select * 
from uosoffering a, lecture b, classroom c
where a.uoscode = b.uoscode and a.semester = b.semester and a.year = b.year 
and b.classroomid = c.classroomid and a.maxenrollment < maximum and a.uoscode = course and c.type != classtype and a.semester = in_semester and a.year = in_year
OR 
((ROW(a.semester, a.uoscode, a.year) NOT IN
        (
        SELECT  l.semester, l.uoscode, l.Year
        FROM    lecture l
        )) AND (a.semester = in_semester and a.year = in_year)AND  a.maxenrollment < maximum and a.uoscode = course ) ;

commit;
end

$$
DELIMITER ;