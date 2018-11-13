DELIMITER $$

create trigger q6
after insert on made_money
for each row
begin
insert into log_data
values (new.movie, new.category);
end;

$$
DELIMITER ;