DELIMITER $$

create trigger q4_1
before insert on made_money
for each row
begin
if (new.how_much < 1000 and new.how_much > 3000000000)
then 
SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error - 1';
else if (new.category = "Action" and new.how_much < 10000)
then
SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error - 2';
else if (new.category = "Comedy" and new.how_much > 1000000000)
then
SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error - 3'; 
end if;
end if;
end if;
end;

$$
DELIMITER ;

DELIMITER $$

create trigger q4_2
before update on made_money
for each row
begin
if (new.how_much < 1000 and new.how_much > 3000000000)
then 
SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error - 1';
else if (new.category = "Action" and new.how_much < 10000)
then
SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error - 2';
else if (new.category = "Comedy" and new.how_much > 1000000000)
then
SIGNAL SQLSTATE 'ERROR' SET MESSAGE_TEXT = 'Constraint Error - 3'; 
end if;
end if;
end if;
end;

$$
DELIMITER ;


