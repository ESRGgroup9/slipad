DROP DATABASE IF EXISTS `slipad`;
CREATE DATABASE `slipad`;
USE `slipad`;

DROP TABLE IF EXISTS `operator`;
CREATE TABLE `operator`(
	`id` INTEGER UNIQUE NOT NULL AUTO_INCREMENT,
	`name` CHAR(50) NOT NULL,
	`password` CHAR(50) NOT NULL,
	PRIMARY KEY(`id`)
);

DROP TABLE IF EXISTS `region`;
CREATE TABLE `region`(
	`post_code` CHAR(8) NOT NULL CHECK(`post_code` LIKE '____-___'),
	`operator_id` INTEGER,
	`parish` CHAR(50) NOT NULL,
	`county` CHAR(50) NOT NULL,
	`district` CHAR(50) NOT NULL,
	PRIMARY KEY(`post_code`),
	FOREIGN KEY(`operator_id`) REFERENCES `operator`(`id`)
		ON UPDATE CASCADE
);

DROP TABLE IF EXISTS `location`;
CREATE TABLE `location`(
	`id` INTEGER AUTO_INCREMENT,
	`latitude` DECIMAL(8,6) NOT NULL,
	`longitude` DECIMAL(9,6) NOT NULL,
	`post_code` CHAR(8) NOT NULL,
	`street_name` CHAR(50) NOT NULL,
	CONSTRAINT `UC_Coords` UNIQUE (`latitude`,`longitude`),
	PRIMARY KEY(`id`),
	FOREIGN KEY(`post_code`) REFERENCES `region`(`post_code`)
		ON UPDATE CASCADE
);

DROP TABLE IF EXISTS `lamppost`;
CREATE TABLE `lamppost`(
	`id` INTEGER,
	`address` INTEGER UNIQUE NOT NULL,
	`status` ENUM('FAIL', 'OFF', 'ON', 'MIN') DEFAULT('OFF'),
	`gateway_sd` INTEGER DEFAULT(-1),
	PRIMARY KEY(`id`),
	FOREIGN KEY(`id`) REFERENCES `location`(`id`)
		ON UPDATE CASCADE
);

DROP TABLE IF EXISTS `parking_space`;
CREATE TABLE `parking_space`(
	`id` INTEGER,
	`num_vacants` INTEGER DEFAULT(0) CHECK(`num_vacants`>=0),
	PRIMARY KEY(`id`),
	FOREIGN KEY(`id`) REFERENCES `lamppost`(`id`)
		ON UPDATE CASCADE
);

-- ----------------------------------------------------------------------------

SET GLOBAL log_bin_trust_function_creators = 1;
DELIMITER $$

-- ------------ Triggers ------------

DROP TRIGGER IF EXISTS insert_parking$$
CREATE TRIGGER insert_parking
	AFTER INSERT ON lamppost
	FOR EACH ROW
	BEGIN
		DECLARE park_id INTEGER;
		
		SELECT NEW.id INTO park_id;
		INSERT INTO parking_space(id) VALUES(park_id);
	END$$

-- ------------ Procedures ------------

-- DROP PROCEDURE IF EXISTS update_lamppost_status$$

-- CREATE PROCEDURE update_lamppost_status(
-- 	`stat` ENUM('FAIL', 'OFF', 'ON', 'MIN'),
-- 	`lamppostID` INTEGER)
-- BEGIN
-- 	UPDATE lamppost SET status=stat WHERE id=lamppostID;
-- END$$

DROP PROCEDURE IF EXISTS dynamic_light_lamppost$$
CREATE PROCEDURE dynamic_light_lamppost(`lamppostID` INTEGER)
BEGIN
	DECLARE next_id, prev_id INTEGER;

	SELECT id INTO next_id FROM lamppost WHERE id=(SELECT MIN(id) FROM lamppost WHERE id>lamppostID);
	SELECT id INTO prev_id FROM lamppost WHERE id=(SELECT MAX(id) FROM lamppost WHERE id<lamppostID);
	UPDATE lamppost set status='ON' where id IN (next_id, prev_id);
END$$

-- ----------------------------------------------------------------------------

DELIMITER ;

-- ----------------------------------------------------------------------------
