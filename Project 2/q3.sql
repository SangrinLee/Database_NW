DELIMITER $$

create trigger q3_1
before insert on married
for each row
begin
if ((select b.star from in_couple b where b.couple_num = new.couple_num limit 1)
in (select a.star from in_couple a where a.couple_num in (select b.couple_num from married b))
or
(select b.star from in_couple b where b.couple_num = new.couple_num limit 1, 1)
in (select a.star from in_couple a where a.couple_num in (select b.couple_num from married b)))
then
	SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error !!!!'; 
end if;
end;

$$
DELIMITER ;

DELIMITER $$

create trigger q3_2
before update on married
for each row
begin
if ((select b.star from in_couple b where b.couple_num = new.couple_num limit 1)
in (select a.star from in_couple a where a.couple_num in (select b.couple_num from married b))
or
(select b.star from in_couple b where b.couple_num = new.couple_num limit 1, 1)
in (select a.star from in_couple a where a.couple_num in (select b.couple_num from married b)))
then
	SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error !!!!'; 
end if;
end;

$$
DELIMITER ;
