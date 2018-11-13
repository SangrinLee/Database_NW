DELIMITER $$

create trigger q1_1
before insert on made_money
for each row
begin
if new.category != "Romantic" and new.category != "Comedy" and new.category != "Drama" and new.category != "Action"
then
set new.category = "Action";
end if;
end;
$$

DELIMITER ;

DELIMITER $$

create trigger q1_2
before update on made_money
for each row
begin
if new.category != "Romantic" and new.category != "Comedy" and new.category != "Drama" and new.category != "Action"
then
set new.category = "Action";
end if;
end;
$$

DELIMITER ;



