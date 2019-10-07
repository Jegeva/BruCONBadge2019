
create view b_type_power_v
as

select
beeromon_type.id as bid,
bname,
beeromon_power.id as pid,
beeromon_power.name as pname ,
beeromon_power_type.id as tid,
typename as ptype
from
beeromon_power, beeromon_type, beeromon_power_by_type, beeromon_power_type
where
beeromon_power_by_type.type_id = beeromon_type.id and
beeromon_power_by_type.power_id = beeromon_power.id and
beeromon_power.ptype = beeromon_power_type.id
order by bname
;

create view b_type_element_v
as
select
beeromon_type.id,
beeromon_type.bname,
beeromon_type.element,

beeromon_element.ename
from
beeromon_type, beeromon_element
where
beeromon_type.element = beeromon_element.id
;
