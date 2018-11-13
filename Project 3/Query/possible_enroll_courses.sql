#SELECT * FROM student ;

drop procedure if exists possible_enroll_courses;

DELIMITER $$

create procedure possible_enroll_courses (in in_id int, in this_year int, in this_sem char(20), in next_year int, in next_sem char(20))
begin
start transaction;

if (((select MaximumPreferredEnrollment from student where id = in_id)  is null)&&((select NonPreferredClassroomType from student where id = in_id) is null))
then

select * from uosoffering a, lecture b, classroom c
where a.uoscode = b.uoscode and a.semester = b.semester and a.year = b.year and b.classroomid = c.classroomid
 #and a.maxenrollment > (select MaximumPreferredEnrollment from student where id = in_id) 
 And ((a.semester = this_sem and a.year = this_year) or (a.semester = next_sem and a.year = next_year));


else if ((select MaximumPreferredEnrollment from student where id = in_id)  is null)
then


SELECT * FROM UOSOFFERING WHERE ((semester = this_sem and year = this_year) or (semester = next_sem and year = next_year)) AND uoscode in
(select uoscode from lecture where ((semester = this_sem and year = this_year) or (semester = next_sem and year = next_year)) AND (ClassroomId  in
(select ClassroomId from classroom where type!=(select NonPreferredClassroomType from student where id = in_id)))) OR 
(ROW(semester, uoscode, Year) NOT IN
        (
        SELECT  l.semester, l.uoscode, l.Year
        FROM    lecture l
        ) AND ((semester = this_sem and year = this_year) or (semester = next_sem and year = next_year))) ;
         


else if ((select NonPreferredClassroomType from student where id = in_id) is null)
then
select * 
from uosoffering a, lecture b, classroom c
where a.uoscode = b.uoscode and a.semester = b.semester and a.year = b.year and b.classroomid = c.classroomid
 and a.maxenrollment <= (select MaximumPreferredEnrollment from student where id = in_id) 
 #and c.type != (select NonPreferredClassroomType from student where id = in_id)
 and ((a.semester = this_sem and a.year = this_year) or (a.semester = next_sem and a.year = next_year));
 
 
 else
SELECT * FROM UOSOFFERING WHERE ((semester = this_sem and year = this_year) or (semester = next_sem and year = next_year)) AND 
maxenrollment <= (select MaximumPreferredEnrollment from student where id = in_id) 
AND uoscode in
(select uoscode from lecture where ((semester = this_sem and year = this_year) or (semester = next_sem and year = next_year)) AND (ClassroomId  in
(select ClassroomId from classroom where type!=(select NonPreferredClassroomType from student where id = in_id) ) ) ) OR 
((ROW(semester, uoscode, Year) NOT IN
        (
        SELECT  l.semester, l.uoscode, l.Year
        FROM    lecture l
        ) AND ((semester = this_sem and year = this_year) or (semester = next_sem and year = next_year)))  AND 
maxenrollment <= (select MaximumPreferredEnrollment from student where id = in_id) );
         
 
 

 
end if;

end if;
end if;
commit;
end

$$
DELIMITER ;













#CALL possible_enroll_courses(5123,2016,"Q1", 2017, "Q2");









