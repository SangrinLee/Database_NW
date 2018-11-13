DELIMITER $$

create trigger q5_1
before insert on divorced
for each row
begin
if (date(NEW.day) < (select date(b.day)	from married b
	where b.couple_num = new.couple_num))
then
set new.day = (select date(b.day) from married b where b.couple_num = new.couple_num);
end if;
end;

$$
DELIMITER ;

DELIMITER $$

create trigger q5_2
before update on divorced
for each row
begin
if (date(NEW.day) < (select date(b.day)	from married b
	where b.couple_num = new.couple_num))
then
set new.day = (select date(b.day) from married b where b.couple_num = new.couple_num);
end if;
end;

$$
DELIMITER ;