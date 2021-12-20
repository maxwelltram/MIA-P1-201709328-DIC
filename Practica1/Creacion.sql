USE Practica1;

CREATE TABLE CountryCodes(
    id int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name varchar(50) NOT NULL UNIQUE,
    name_name varchar(50) NOT NULL,
    name_aiddata_code int(11) NULL,
    name_aiddata_name varchar(50) NULL,
    name_cow_alpha varchar(15) NULL,
    name_cow_numeric int(5) NULL,
    name_fao_code int(5) NULL,
    name_fips varchar(6) NULL, 
    name_geonames_id int(11) NULL,
    name_imf_code int(11) NULL,
    name_iso2 varchar(3) NULL,
    name_iso3 varchar(4) NULL,
    name_iso_numeric int(11) NULL,
    name_oecd_code int(11) NULL,
    name_oecd_name varchar(50) NULL,
    name_un_code int(11) NULL,
    name_wb_code varchar(6) NULL
);

CREATE TABLE Currenci(
    idCurrency int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name_Currency varchar(15) NOT NULL UNIQUE
);

CREATE TABLE geoName(
    id int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    idGeoname int(11) NOT NULL,
    place_name varchar(100) NOT NULL,
    latitude decimal(16,5) NOT NULL,
    longitude decimal(16,5) NOT NULL,
    location_type_code int(11) NOT NULL,
    gazetter_adm_code varchar(250) NOT NULL,
    gazetter_adm_name varchar(250) NOT NULL,
    location_class int(7) NOT NULL, 
    geographic_exactness int(2) NOT NULL
);

CREATE TABLE level(
    id int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    project_id int(11) NOT NULL,
    project_location_id varchar(30) NOT NULL,
    geoname_id int(11) NOT NULL,
    transactions_start_year int(11) NOT NULL,
    transactions_end_year int(11) NOT NULL,
    even_split_commitments decimal(16,5) NULL,
    even_split_disbursements decimal(16,5) NULL 
);

CREATE TABLE locations(
    IdLocation int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    location_type_code varchar(6) NOT NULL UNIQUE,
    location_type_name varchar(250) NOT NULL
);

CREATE TABLE project(
    idProyecto int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    project_id varchar(15) NOT NULL UNIQUE,
    id_geocoded int(2) NOT NULL,
    project_title varchar(250) NOT NULL,
    start_actual_isodate date NOT NULL,
    end_actual_isodate date NULL,
    donors varchar(15) NOT NULL,
    donors_iso3 varchar(5) NOT NULL, 
    recipients int(11) NOT NULL,
    recipients_iso3 varchar(4) NOT NULL,
    ad_sector_codes varchar(100) NOT NULL,
    ad_sector_names varchar(300) NOT NULL,
    status int(11) NOT NULL,
    transactions_start_year int(11) NOT NULL,
    transactions_end_year int(11) NOT NULL,
    total_commitments decimal(16,5) NOT NULL,
    total_disbursements decimal(16,5) NULL

);

CREATE TABLE Status(
    idStatus int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    Name varchar(50) NOT NULL UNIQUE
);

CREATE TABLE Transaction(
    id int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    id_Transaction varchar(25) NOT NULL UNIQUE,
    idProyecto int(11) NOT NULL,
    transaction_isodate date NOT NULL,
    transaction_year date NOT NULL,
    transaction_value_code varchar(5) NOT NULL,
    transaction_currency int NOT NULL,
    transaction_value decimal(16,5) NOT NULL
);



ALTER TABLE Transaction
ADD FOREIGN KEY (transaction_currency) REFERENCES Currenci(idCurrency);

ALTER TABLE Transaction
ADD FOREIGN KEY (idProyecto) REFERENCES project(idProyecto);

ALTER TABLE project
ADD FOREIGN KEY (recipients)  REFERENCES CountryCodes(id);

ALTER TABLE level
ADD FOREIGN KEY (project_id) REFERENCES project(idProyecto);

ALTER TABLE level 
ADD FOREIGN KEY (geoname_id) REFERENCES geoName(id);

ALTER TABLE geoName
ADD FOREIGN KEY (location_type_code) REFERENCES locations(idLocation);