# check: https://www.codigo-postal.pt/?cp4=4800&cp3=

# operator: ID, name, password
INSERT INTO operator(id, name, password) VALUES
(NULL, 'Joao', 'pass1'),
(NULL, 'Ana', 'pass2'),
(NULL, 'Jose', 'pass3'),
(NULL, 'Rui', 'pass4');

# region: post_code, operatorID, parish, county, district
INSERT INTO region(post_code, operator_id, parish, county, district) VALUES
-- ('4800-015', 1, 'Aldao', 'Guimaraes', 'Braga'),
-- ('4800-424', 1, 'Gominhaes', 'Guimaraes', 'Braga'),
('4800-073', 2, 'Azurem', 'Guimaraes', 'Braga'),
('4810-420', 2, 'Azurem', 'Guimaraes', 'Braga'),
('4800-045', 2, 'Azurem', 'Guimaraes', 'Braga');

# location: id, latitude, longitude, post_code, street_name
INSERT INTO location(id, latitude, longitude, post_code, street_name) VALUES
-- (1, 41.457564, -8.279383, '4800-015', 'Travessa de Sao Mamede'),
(2, 41.447849, -8.298462, '4800-073', 'Rua Teixeira de Pascoais'),
(3, 41.448042, -8.298266, '4800-073', 'Rua Teixeira de Pascoais'),
(4, 41.448259, -8.298030, '4800-073', 'Rua Teixeira de Pascoais'),
(5, 41.448579, -8.297638, '4800-073', 'Rua Teixeira de Pascoais'),
(6, 41.448726, -8.297477, '4800-073', 'Rua Teixeira de Pascoais'),
(7, 41.448867, -8.297321, '4800-073', 'Rua Teixeira de Pascoais'),

(8, 41.446749, -8.298788, '4810-420', 'Alameda Dr. Alfredo Pimenta'),
(9, 41.446616, -8.298722, '4810-420', 'Alameda Dr. Alfredo Pimenta'),
(10, 41.446407, -8.298593, '4810-420', 'Alameda Dr. Alfredo Pimenta'),
(11, 41.446294, -8.298529, '4810-420', 'Alameda Dr. Alfredo Pimenta'),
(12, 41.446169, -8.298470, '4810-420', 'Alameda Dr. Alfredo Pimenta'),

(13, 41.447986, -8.300469, '4800-045', 'Alameda Dr. Alfredo Pimenta'),
(14, 41.448090, -8.300711, '4800-045', 'Alameda Dr. Alfredo Pimenta'),
(15, 41.448154, -8.300909, '4800-045', 'Alameda Dr. Alfredo Pimenta'),
(16, 41.448259, -8.301166, '4800-045', 'Alameda Dr. Alfredo Pimenta'),
(17, 41.448331, -8.301386, '4800-045', 'Alameda Dr. Alfredo Pimenta');

# lamppost: locationID, address, status
INSERT INTO lamppost(id, address) VALUES
-- (1, 0xa1),
(2, 0xa2),
(3, 0xa3),
(4, 0xa4),
(5, 0xa5),
(6, 0xa6),
(7, 0xa7),

(8, 0xa8),
(9, 0xa9),
(10,0xaa),
(11,0xab),
(12,0xac),

(13, 0xad),
(14, 0xae),
(15, 0xaf),
(16, 0xb0),
(17, 0xb1);

# trigger inserts Parking space when insert on lamppost is done
