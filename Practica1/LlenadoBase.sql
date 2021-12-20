SET GLOBAL local_infile=1; 

load data local infile '/home/maxwelltram/Escritorio/Practica1/transactions.csv'
into table transactions_temp
CHARACTER SET UTF8
COLUMNS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(campo_1, campo_2, @transaction_isodate, @campo_4,
campo_5, campo_6, campo_7)
SET campo_3 = STR_TO_DATE(@transactionisodate, '%m/%d/%Y');
SET transaction_year = STR_TO_DATE(@campo_4 ,'%Y');


load data local infile '/home/maxwelltram/Escritorio/Practica1/locations.csv'
into table locations_temp
CHARACTER SET UTF8
COLUMNS TERMINATED BY '\t'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(campo_1, campo_3);


load data local infile '/home/maxwelltram/Escritorio/Practica1/level_1a.csv'
into table level_temp
CHARACTER SET UTF8
COLUMNS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(campo_1,campo_2, campo_3,campo_4,campo_5,campo_6,campo_7);


load data local infile '/home/maxwelltram/Escritorio/Practica1/geonames.csv'
into table geonames_temp
CHARACTER SET UTF8
COLUMNS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(campo_1,campo_2, campo_3,campo_4,campo_5,campo_6,campo_7,campo_8,campo_9);


load data local infile '/home/maxwelltram/Escritorio/Practica1/geonames.csv'
into table geonames_temp
CHARACTER SET UTF8
COLUMNS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(campo_1,campo_2, campo_3,campo_4,campo_5,campo_6,campo_7,campo_8,campo_9);


load data local infile '/home/maxwelltram/Escritorio/Practica1/projects.csv'
into table project_temp
CHARACTER SET UTF8
COLUMNS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(campo_1,campo_2, campo_3,@campo_4,@campo_5,campo_6,campo_7,campo_8,campo_9
,campo_10,campo_11,campo_12,campo_13,campo_14,campo_15,campo_16);
SET campo_4 = STR_TO_DATE(@campo_4, '%m/%d/%Y');
SET campo_5 = STR_TO_DATE(@campo_5, '%m/%d/%Y');




load data local infile '/home/maxwelltram/Escritorio/Practica1/country_codes.tsv'
into table country_codes_temp
CHARACTER SET UTF8
COLUMNS TERMINATED BY '\t'
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(campo_1,campo_2, campo_3,campo_4,campo_5,campo_6,campo_7,campo_8,campo_9,
campo_10,campo_11,campo_12,campo_13,campo_14,campo_15,campo_16, campo_17,campo_18);







