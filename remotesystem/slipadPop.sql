# check: https://www.codigo-postal.pt/?cp4=4800&cp3=

# operator: ID, name, password
INSERT INTO operator VALUES(1, 'Joao', 'pass1');
INSERT INTO operator VALUES(2, 'Ana', 'pass2');
INSERT INTO operator VALUES(3, 'Jose', 'pass3');
INSERT INTO operator VALUES(4, 'Rui', 'pass4');

# region: post_code, operatorID, parish, county, district
INSERT INTO region VALUES('4800-015', 1, 'Aldao', 'Guimaraes', 'Braga');
INSERT INTO region VALUES('4800-424', 1, 'Gominhaes', 'Guimaraes', 'Braga');
INSERT INTO region VALUES('4800-040', 2, 'Azurem', 'Guimaraes', 'Braga');
INSERT INTO region VALUES('4800-136', 3, 'Fermentoes', 'Guimaraes', 'Braga');
INSERT INTO region VALUES('4800-245', 4, 'Ataes', 'Guimaraes', 'Braga');

# location: id, latitude, longitude, post_code, street_name
INSERT INTO location VALUES(1, 41.457564, -8.279383, '4800-015', 'Travessa de Sao Mamede');
INSERT INTO location VALUES(2, 41.444423, -8.294082, '4800-424', 'Rua Adelaide Carvalho');
INSERT INTO location VALUES(3, 38.806669, -9.227690, '4800-040', 'Travessa Doutor Mario Dias');
INSERT INTO location VALUES(4, 41.066845, -8.576422, '4800-136', 'Rua Arquitecto Fernando Tavora');
INSERT INTO location VALUES(5, 41.465706, -8.247396, '4800-245', "Rua do Parque");

# lamppost: locationID, address, status
INSERT INTO lamppost VALUES(1, 0xbb, 'OFF');
INSERT INTO lamppost VALUES(2, 0xcc, 'OFF');
INSERT INTO lamppost VALUES(3, 3, 'OFF');
INSERT INTO lamppost VALUES(4, 4, 'OFF');
INSERT INTO lamppost VALUES(5, 5, 'OFF');

# parking_space: lamppostID, num_vacants
INSERT INTO parking_space VALUES(1, 0);
INSERT INTO parking_space VALUES(2, 0);
INSERT INTO parking_space VALUES(3, 0);
INSERT INTO parking_space VALUES(4, 0);
INSERT INTO parking_space VALUES(5, 0);
