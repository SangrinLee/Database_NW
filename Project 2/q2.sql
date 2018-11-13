DELIMITER $$

create trigger q2_1
before insert on appeared_in
for each row
begin
if(new.star not in 
(select d.star from appeared_in d, made_money e
where d.movie = e.movie and
(e.category = "Romantic" or e.category = "Comedy" or e.category = "Drama")))
then
update made_money f
set f.category = "Drama"
where f.movie = new.movie;
end if;
end;

$$
DELIMITER ;

DELIMITER $$

create trigger q2_2
before update on appeared_in
for each row
begin
if(new.star not in 
(select d.star from appeared_in d, made_money e
where d.movie = e.movie and
(e.category = "Romantic" or e.category = "Comedy" or e.category = "Drama")))
then
update made_money f
set f.category = "Drama"
where f.movie = new.movie;
end if;
end;

$$
DELIMITER ;