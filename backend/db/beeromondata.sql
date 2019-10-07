select 'i beeromons' ;

insert into beeromon_element values(0,'Hop');   -- strong against wheat , weak against water
insert into beeromon_element values(1,'Wheat'); -- strong against water , weak against yeast
insert into beeromon_element values(2,'Water'); -- strong against triple, weak against hop
insert into beeromon_element values(3,'Triple');-- strong against hop   , weak against wheat
insert into beeromon_element values(4,'0x90');   -- volunteers and team will be given a special beeromon strong against all

insert into beeromon_type values (0,'Karmelieto'      ,'karmelieto.pic'   ,0);
insert into beeromon_type values (1,'Bruugsausaur'    ,'karmelieto.pic'   ,1);
insert into beeromon_type values (2,'Kriektle'        ,'karmelieto.pic'   ,2);
insert into beeromon_type values (3,'Vedetteee'       ,'karmelieto.pic'   ,3);
insert into beeromon_type values (4,'Orvala'          ,'karmelieto.pic'   ,0);
insert into beeromon_type values (5,'Chimayyll'       ,'karmelieto.pic'   ,1);
insert into beeromon_type values (6,'Westmallo'       ,'karmelieto.pic'   ,2);
insert into beeromon_type values (7,'GuldeenDrakken'  ,'karmelieto.pic'   ,3);
insert into beeromon_type values (8,'Arenddd'         ,'karmelieto.pic'   ,0);
insert into beeromon_type values (9,'Chouffinu'       ,'karmelieto.pic'   ,1);
insert into beeromon_type values (10,'Omerus'          ,'karmelieto.pic'   ,2);
insert into beeromon_type values (11,'OudeKrieky'      ,'karmelieto.pic'   ,3);
insert into beeromon_type values (12,'Rochefurt'       ,'karmelieto.pic'   ,0);
insert into beeromon_type values (13,'Malheuri'        ,'karmelieto.pic'   ,1);
insert into beeromon_type values (14,'Palmmur'         ,'karmelieto.pic'   ,2);
insert into beeromon_type values (15,'Nocturno'        ,'karmelieto.pic'   ,3);

insert into beeromon_type values (99,'EternalBlonde'   ,'karmelieto.pic'   ,4); -- you know ;) volunteers
insert into beeromon_type values (100,'Z3r0Day'         ,'karmelieto.pic'   ,4); -- you know ;) team
insert into beeromon_type values (101,'C40l1L4'         ,'karmelieto.pic'   ,4); -- you know ;) badge team

insert into beeromon_power_type values (0, 'ferment' );  -- atk
insert into beeromon_power_type values (1, 'pressure' ); -- def
insert into beeromon_power_type values (2, 'fill' );     -- hp
insert into beeromon_power_type values (3, 'infect' );   -- status to ennemy
insert into beeromon_power_type values (4, 'brew' );     -- status to self



insert into beeromon_power values (0,'Drink!'        ,0);
insert into beeromon_power values (1,'Strong batch'  ,1);
insert into beeromon_power values (2,'Double ferment',2);
insert into beeromon_power values (3,'Hangover'      ,3);
insert into beeromon_power values (4,'IPA test'      ,4);

insert into beeromon_power values (5,'Right Glass'   ,0);
insert into beeromon_power values (6,'Bubble swirl'  ,1);
insert into beeromon_power values (7,'Cold one!'     ,2);
insert into beeromon_power values (8,'Thirsty'       ,3);
insert into beeromon_power values (9,'Good year'     ,4);

insert into beeromon_power values (10,'Pintje'        ,0);
insert into beeromon_power values (11,'Master brewer' ,1);
insert into beeromon_power values (12,'Craft beer'    ,2);
insert into beeromon_power values (13,'Pro pour'      ,3);
insert into beeromon_power values (14,'Bad pour'      ,4);

insert into beeromon_power values (15,'How its made'   ,0);
insert into beeromon_power values (16,'Trappist'       ,1);
insert into beeromon_power values (17,'Ptite soeur'    ,2);
insert into beeromon_power values (18,'Happy hour'     ,3);
insert into beeromon_power values (19,'Low pressure'   ,4);

select 'stats.sql';
# perl -e 'for($i=0;$i<16;$i++){$a=1+int(rand(3));$b=1+int(rand(3));$c=(7-$a-$b);print "insert into beeromon_stat values($i,$a,$b,$c);\n"};' > stats.sql
source stats.sql;

select 'powers.sql';
# perl -e 'for($i=0;$i<16;$i++){for($j=0;$j<3;$j++){$p=int(rand(20));print "insert into beeromon_power_by_type values($i,$p);\n"}}' > powers.sql
source powers.sql;

select 'views.sql';
source views.sql;
