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

-- ------------ Triggers ------------
DELIMITER $$

DROP TRIGGER IF EXISTS insert_parking$$

CREATE TRIGGER insert_parking
	AFTER INSERT ON lamppost
	FOR EACH ROW
	BEGIN
		DECLARE park_id INTEGER;
		
		SELECT NEW.id INTO park_id;
		INSERT INTO parking_space(id) VALUES(park_id);
	END$$

DELIMITER ;