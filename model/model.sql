SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `impact` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
USE `impact` ;

-- -----------------------------------------------------
-- Table `impact`.`actyunits`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`actyunits` (
  `UNIT_COD` CHAR(3) NOT NULL ,
  `UNIT_DES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LZH` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LSW` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LRU` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LPT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LNL` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LKO` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LJA` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LIT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LFR` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_LDE` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `UNIT_ABR` VARCHAR(20) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`UNIT_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`activity`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`activity` (
  `ACTY_COD` CHAR(3) NOT NULL ,
  `UNIT_COD` CHAR(3) NULL ,
  `ACTY_DES` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LZH` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LSW` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LRU` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LPT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LNL` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LKO` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LJA` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LIT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LES` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_LDE` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `ACTY_TYP` CHAR(1) NULL DEFAULT 'C' ,
  `ACTY_CTY` CHAR(1) NULL DEFAULT 'O' ,
  `ACTY_IO` CHAR(1) NULL DEFAULT 'I' ,
  PRIMARY KEY (`ACTY_COD`) ,
  INDEX `MNTIDX001` (`UNIT_COD` ASC) ,
  CONSTRAINT `fk_e688edd2-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`UNIT_COD` )
    REFERENCES `impact`.`actyunits` (`UNIT_COD` )
    ON DELETE CASCADE
    ON UPDATE SET NULL)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`actymethods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`actymethods` (
  `ACTY_COD` CHAR(3) NOT NULL ,
  `MET_COD` VARCHAR(5) NOT NULL ,
  `MET_DES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LZH` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LSW` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LRU` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LPT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LNL` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LKO` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LJA` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LIT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LFR` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `MET_LDE` VARCHAR(60) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`ACTY_COD`, `MET_COD`) ,
  INDEX `MNTIDX002` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_e67e4fee-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`country`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`country` (
  `CNTY_COD` CHAR(3) NOT NULL ,
  `CNTY_NAM` VARCHAR(20) NULL DEFAULT 'Without name' ,
  `CURRDES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CURRSYMBOL` VARCHAR(8) NULL DEFAULT 'No Sym' ,
  `CURRCODE` VARCHAR(4) NULL DEFAULT 'NONE' ,
  PRIMARY KEY (`CNTY_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`cpprods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cpprods` (
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LZH` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LSW` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LRU` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LPT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LNL` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LKO` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LJA` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LIT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LFR` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PROD_LDE` VARCHAR(60) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`PROD_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crop`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crop` (
  `CROP_COD` CHAR(3) NOT NULL ,
  `CROP_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `VEGETABLE` CHAR(1) NULL DEFAULT 'N' ,
  `SHOWINLIST` CHAR(1) NULL DEFAULT 'Y' ,
  PRIMARY KEY (`CROP_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`forages`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`forages` (
  `FORA_COD` CHAR(3) NOT NULL ,
  `FORA_DES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LZH` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LSW` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LRU` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LPT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LNL` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LKO` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LJA` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LIT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LFR` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `FORA_LDE` VARCHAR(60) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`FORA_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`cropfora`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropfora` (
  `CROP_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `USDAR` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `USDAC` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `SNITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ASH` DECIMAL(7,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`CROP_COD`, `FORA_COD`) ,
  INDEX `MNTIDX013` (`CROP_COD` ASC) ,
  INDEX `MNTIDX014` (`FORA_COD` ASC) ,
  CONSTRAINT `fk_e680c62a-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROP_COD` )
    REFERENCES `impact`.`crop` (`CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e6818d8a-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`FORA_COD` )
    REFERENCES `impact`.`forages` (`FORA_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`cropprods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropprods` (
  `CROP_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(7,4) NULL DEFAULT 0 ,
  `USADR` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `USADC` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `SNITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ASH` DECIMAL(7,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`CROP_COD`, `PROD_COD`) ,
  INDEX `MNTIDX011` (`CROP_COD` ASC) ,
  INDEX `MNTIDX012` (`PROD_COD` ASC) ,
  CONSTRAINT `fk_e67f0178-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROP_COD` )
    REFERENCES `impact`.`crop` (`CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e67fbf1e-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`PROD_COD` )
    REFERENCES `impact`.`cpprods` (`PROD_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`drainage`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`drainage` (
  `DRAIN_COD` VARCHAR(5) NOT NULL ,
  `DRAIN_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DRAIN_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`DRAIN_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`ferttype`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`ferttype` (
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `FERTYP_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FERTYP_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `MANURE` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`FERTYP_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`hcompprods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`hcompprods` (
  `PROD_COD` VARCHAR(5) NOT NULL ,
  `PROD_TYP` VARCHAR(20) NULL ,
  `PROD_DES` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LZH` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LSW` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LRU` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LPT` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LNL` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LKO` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LJA` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LIT` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LFR` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LES` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `PROD_LDE` VARCHAR(85) NULL DEFAULT 'Without description' ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`PROD_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livestoc`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livestoc` (
  `LIVE_COD` CHAR(3) NOT NULL ,
  `LIVE_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LIVE_TPY` CHAR(1) NULL DEFAULT 'R' ,
  PRIMARY KEY (`LIVE_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`products`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`products` (
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LZW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `PROD_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `INDNANIM` CHAR(1) NULL DEFAULT 'N' ,
  `INDWEIGHT` CHAR(1) NULL DEFAULT 'N' ,
  `INDPRICE` CHAR(1) NULL DEFAULT 'Y' ,
  `INDQTY` CHAR(1) NULL DEFAULT 'Y' ,
  `HHCONS` CHAR(1) NULL DEFAULT 'Y' ,
  `BYCATE` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`PROD_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`lvproducts`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`lvproducts` (
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(7,3) NULL DEFAULT 0 ,
  `USDAR` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `SALESBY` CHAR(1) NULL DEFAULT 'Q' ,
  `WEIGHT` CHAR(1) NULL DEFAULT 'N' ,
  `HHCONS` CHAR(1) NULL DEFAULT 'Y' ,
  `HERD` CHAR(1) NULL DEFAULT 'N' ,
  `MANURE` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`LIVE_COD`, `PROD_COD`) ,
  INDEX `MNTIDX003` (`LIVE_COD` ASC) ,
  INDEX `MNTIDX004` (`PROD_COD` ASC) ,
  CONSTRAINT `fk_e6857184-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`LIVE_COD` )
    REFERENCES `impact`.`livestoc` (`LIVE_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e68642f8-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`PROD_COD` )
    REFERENCES `impact`.`products` (`PROD_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`markoutlet`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`markoutlet` (
  `MARK_COD` CHAR(3) NOT NULL ,
  `MARK_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `MARK_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`MARK_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`pastures`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`pastures` (
  `CROP_COD` CHAR(3) NOT NULL ,
  `CROP_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `DETAIL` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`CROP_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`pastfora`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`pastfora` (
  `CROP_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `USDAR` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `USDAC` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `SNITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ASH` DECIMAL(7,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`CROP_COD`, `FORA_COD`) ,
  INDEX `MNTIDX009` (`CROP_COD` ASC) ,
  INDEX `MNTIDX010` (`FORA_COD` ASC) ,
  CONSTRAINT `fk_e683dda6-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROP_COD` )
    REFERENCES `impact`.`pastures` (`CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e684a4ca-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`FORA_COD` )
    REFERENCES `impact`.`forages` (`FORA_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`restypes`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`restypes` (
  `RES_COD` CHAR(3) NOT NULL ,
  `RES_CAT` VARCHAR(60) NULL DEFAULT 'Without category' ,
  `RES_DES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LZH` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LSW` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LRU` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LPT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LNL` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LKO` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LJA` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LIT` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LFR` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `RES_LDE` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`RES_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`soil`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`soil` (
  `SOIL_COD` VARCHAR(5) NOT NULL ,
  `LOCA_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `LOCA_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `USDA_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `FAO_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`SOIL_COD`) )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`systype`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`systype` (
  `SYST_COD` CHAR(3) NOT NULL ,
  `SYST_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LZH` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LSW` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LRU` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LPT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LNL` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LKO` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LJA` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LIT` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LFR` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `SYST_LDE` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYST_COD`) ,
  INDEX `MNTIDX005` (`CROP_COD` ASC) ,
  INDEX `MNTIDX006` (`LIVE_COD` ASC) ,
  CONSTRAINT `fk_e6871a7a-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`LIVE_COD` )
    REFERENCES `impact`.`livestoc` (`LIVE_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e687f8dc-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROP_COD` )
    REFERENCES `impact`.`crop` (`CROP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`pastprods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`pastprods` (
  `CROP_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(7,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(7,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(7,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(7,3) NULL DEFAULT 0 ,
  `USDAR` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `USDAC` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `SNITROGEN` DECIMAL(7,3) NULL DEFAULT 0 ,
  `ASH` DECIMAL(7,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`CROP_COD`, `PROD_COD`) ,
  INDEX `MNTIDX007` (`CROP_COD` ASC) ,
  INDEX `MNTIDX008` (`PROD_COD` ASC) ,
  CONSTRAINT `fk_e6825a62-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROP_COD` )
    REFERENCES `impact`.`pastures` (`CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e6831844-bce0-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`PROD_COD` )
    REFERENCES `impact`.`cpprods` (`PROD_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`system`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`system` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CNTY_COD` CHAR(3) NOT NULL ,
  `SYST_COD` CHAR(3) NOT NULL ,
  `STATE` VARCHAR(20) NULL DEFAULT 'Set state' ,
  `STATEABRE` CHAR(3) NULL DEFAULT 'SST' ,
  `REG_DESC` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `LATI_DEG` VARCHAR(12) NULL DEFAULT '0' ,
  `LAT_CARD` CHAR(1) NULL DEFAULT 'N' ,
  `LONG_DEG` VARCHAR(12) NULL DEFAULT '0' ,
  `LON_CARD` CHAR(1) NULL DEFAULT 'E' ,
  `ELEVAT` DECIMAL(7,2) NULL DEFAULT 0 ,
  `YEAR` VARCHAR(10) NULL DEFAULT '20051224' ,
  `GRASSONLY` CHAR(1) NULL DEFAULT 'N' ,
  `NOTES` VARCHAR(254) NULL DEFAULT 'Some notes' ,
  `DATAOWNER` VARCHAR(30) NULL DEFAULT 'Set data owner' ,
  `DTOWNABRE` CHAR(3) NULL DEFAULT 'NON' ,
  `FARMNAME` VARCHAR(60) NULL DEFAULT 'Set farmers name' ,
  `NUMSEASONS` CHAR(1) NULL DEFAULT '2' ,
  `LOCATION` VARCHAR(40) NULL DEFAULT 'Set a location' ,
  `SUBLOC` VARCHAR(40) NULL DEFAULT 'Set sub-location' ,
  `TIPCAMBIO` DECIMAL(8,4) NULL DEFAULT 0 ,
  `LASTUPD` VARCHAR(30) NULL ,
  `LASTUPDUSR` VARCHAR(20) NULL ,
  `LANGUAGE` VARCHAR(20) NULL DEFAULT 'EN' ,
  `PERSINTERVIEW` VARCHAR(60) NULL ,
  `WHENSTART` VARCHAR(60) NULL ,
  `SINCELIVING` VARCHAR(60) NULL ,
  `IMMIGRATED` VARCHAR(60) NULL ,
  `MAINPROD` VARCHAR(60) NULL ,
  `MPRODCHANGED` VARCHAR(60) NULL ,
  `CROPROT` VARCHAR(250) NULL ,
  `YIELDVARY` VARCHAR(250) NULL ,
  `YIELDVARYRANGE` VARCHAR(60) NULL ,
  `YIELDVARYWHY` VARCHAR(250) NULL ,
  `PRICEVARY` VARCHAR(250) NULL ,
  `PRICEVARYYEAR` VARCHAR(250) NULL ,
  `PRICEVARYWHY` VARCHAR(250) NULL ,
  `LMARKET` VARCHAR(60) NULL ,
  `DLMARKET` DECIMAL(12,3) NULL DEFAULT 0 ,
  `UMARKET` VARCHAR(60) NULL ,
  `DUMARKET` DECIMAL(12,3) NULL DEFAULT 0 ,
  `FARMDRAW` BLOB NULL ,
  `FARMDRAWEXT` CHAR(4) NULL ,
  PRIMARY KEY (`SYS_COD`) ,
  INDEX `SYSIDX005` (`CNTY_COD` ASC) ,
  INDEX `SYSIDX006` (`SYST_COD` ASC) ,
  CONSTRAINT `fk_04d77330-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYST_COD` )
    REFERENCES `impact`.`systype` (`SYST_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_04d7e1e4-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CNTY_COD` )
    REFERENCES `impact`.`country` (`CNTY_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`labforce`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`labforce` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `PERS_TYP` CHAR(2) NULL DEFAULT 'HH' ,
  `PERS_NUM` DECIMAL(3,0) NULL DEFAULT 0 ,
  `AGE` DECIMAL(2,0) NULL DEFAULT 0 ,
  `SEX` CHAR(1) NULL DEFAULT 'M' ,
  `WORK` CHAR(1) NULL DEFAULT 'Y' ,
  `SHORTDESC` VARCHAR(12) NULL DEFAULT 'Without desc' ,
  `EAT` CHAR(1) NULL DEFAULT 'N' ,
  `EDUCATION` VARCHAR(60) NULL DEFAULT 'NONE' ,
  `OCUPATION` VARCHAR(60) NULL DEFAULT 'NONE' ,
  PRIMARY KEY (`SYS_COD`, `PERS_COD`) ,
  INDEX `SYSIDX001` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_04d85584-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`spadstown`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`spadstown` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `SOIL_COD` VARCHAR(5) NULL ,
  `DRAIN_COD` VARCHAR(5) NULL ,
  `PLOT_ID` CHAR(3) NULL ,
  `STATUS` VARCHAR(8) NULL ,
  `AREA` DECIMAL(6,2) NULL DEFAULT 0 ,
  `DISTHH` DECIMAL(6,2) NULL DEFAULT 0 ,
  `NUMSEA` DECIMAL(2,0) NULL DEFAULT 1 ,
  `TYPE` VARCHAR(7) NULL DEFAULT 'C' ,
  `SLOPE` DECIMAL(3,0) NULL DEFAULT 0 ,
  `DEPTH` DECIMAL(4,0) NULL DEFAULT 0 ,
  `LOCALNAME` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `PERMEA` DECIMAL(3,0) NULL DEFAULT 0 ,
  `PORCTOT` DECIMAL(3,0) NULL DEFAULT 0 ,
  `CROPSINCE` CHAR(4) NULL ,
  `LASTFALLOW` CHAR(4) NULL ,
  `CROPLAND` DECIMAL(6,2) NULL DEFAULT 0 ,
  `LANDPOS` VARCHAR(60) NULL ,
  `SLOPECLASS` VARCHAR(60) NULL ,
  `PLOTCOLOR` VARCHAR(60) NULL ,
  `FARMFERTCLASS` VARCHAR(60) NULL ,
  `EROSION` VARCHAR(60) NULL ,
  `CSTRUCTURES` VARCHAR(60) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`) ,
  INDEX `SYSIDX002` (`SYS_COD` ASC) ,
  INDEX `SYSIDX003` (`SOIL_COD` ASC) ,
  INDEX `SYSIDX004` (`DRAIN_COD` ASC) ,
  CONSTRAINT `fk_04d8cf32-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_04d94dfe-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`DRAIN_COD` )
    REFERENCES `impact`.`drainage` (`DRAIN_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_04d9d1f2-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SOIL_COD` )
    REFERENCES `impact`.`soil` (`SOIL_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`lvstosys`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`lvstosys` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PURPOSE` VARCHAR(40) NULL DEFAULT 'DAIRY' ,
  `BREDD` VARCHAR(20) NULL DEFAULT 'Set breed' ,
  `NANIMAL` DECIMAL(7,0) NULL DEFAULT 0 ,
  `NANIMALAS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `BOD_WEIG` DECIMAL(7,0) NULL DEFAULT 0 ,
  `MILKPROD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `LACTLENG` DECIMAL(3,0) NULL DEFAULT 0 ,
  `NLAMB` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MILKFREQ` DECIMAL(3,0) NULL DEFAULT 0 ,
  `CALVINTER` DECIMAL(3,0) NULL DEFAULT 0 ,
  `CALVRATE` DECIMAL(3,0) NULL DEFAULT 0 ,
  `OFFPRING` DECIMAL(5,2) NULL DEFAULT 0 ,
  `REPROMALES` DECIMAL(7,0) NULL DEFAULT 0 ,
  `REPMALESAS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `REPMALEBW` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FEMREPLEC` DECIMAL(7,0) NULL DEFAULT 0 ,
  `WEAAGE` DECIMAL(7,0) NULL DEFAULT 0 ,
  `WEAWEI` DECIMAL(7,0) NULL DEFAULT 0 ,
  `WEAAGENREP` DECIMAL(7,0) NULL DEFAULT 0 ,
  `WEABWNREP` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FIRSTCLAGE` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FIRSTCLWEI` DECIMAL(7,0) NULL DEFAULT 0 ,
  `REPROINTEN` DECIMAL(7,0) NULL DEFAULT 0 ,
  `LAMBYEAR` DECIMAL(7,0) NULL DEFAULT 0 ,
  `NMALETOT` DECIMAL(7,0) NULL DEFAULT 0 ,
  `NFEMTOT` DECIMAL(7,0) NULL DEFAULT 0 ,
  `BODYCOND` VARCHAR(60) NULL ,
  `FEEDSYS` SMALLINT UNSIGNED NULL ,
  `CORRSYS` SMALLINT UNSIGNED NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`) ,
  INDEX `SYSIDX007` (`SYS_COD` ASC) ,
  INDEX `SYSIDX008` (`LIVE_COD` ASC) ,
  CONSTRAINT `fk_04da5a28-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_04dae7f4-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`LIVE_COD` )
    REFERENCES `impact`.`livestoc` (`LIVE_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`wellbeing`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`wellbeing` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `ITEMID` CHAR(3) NOT NULL ,
  `ITEMDESC` VARCHAR(40) NULL DEFAULT 'NONE' ,
  `VERYRICH` VARCHAR(60) NULL ,
  `RICH` VARCHAR(60) NULL ,
  `POOR` VARCHAR(60) NULL ,
  `VERYPOOR` VARCHAR(60) NULL ,
  `RANK` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `ITEMID`) ,
  INDEX `SYSIDX182` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_04db798a-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`plotseasons`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotseasons` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `PORCTOT` DECIMAL(3,0) NULL DEFAULT 0 ,
  `INTER` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`) ,
  INDEX `SYSIDX010` (`SYS_COD` ASC, `PLOT_COD` ASC) ,
  CONSTRAINT `fk_91e67546-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` )
    REFERENCES `impact`.`spadstown` (`SYS_COD` , `PLOT_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`owneduse`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`owneduse` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEGETABLE` CHAR(1) NULL DEFAULT 'N' ,
  `INTER` CHAR(1) NULL DEFAULT 'N' ,
  `LANDCOVER` DECIMAL(3,0) NULL DEFAULT 0 ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`) ,
  INDEX `SYSIDX012` (`CROP_COD` ASC) ,
  INDEX `SYSIDX011` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC) ,
  CONSTRAINT `fk_91e6ee40-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROP_COD` )
    REFERENCES `impact`.`crop` (`CROP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_91e99cf8-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` )
    REFERENCES `impact`.`plotseasons` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`ownusedv`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`ownusedv` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`) ,
  INDEX `SYSIDX014` (`VEG_COD` ASC) ,
  INDEX `SYSIDX013` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC) ,
  CONSTRAINT `fk_91e76a8c-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`VEG_COD` )
    REFERENCES `impact`.`crop` (`CROP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_91ea399c-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    REFERENCES `impact`.`owneduse` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`plotgpspos`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotgpspos` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `POINT_COD` CHAR(3) NOT NULL ,
  `longitude` DECIMAL(12,9) NULL ,
  `latitude` DECIMAL(12,9) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `POINT_COD`) ,
  INDEX `SYSIDX009` (`SYS_COD` ASC, `PLOT_COD` ASC) ,
  CONSTRAINT `fk_91e602aa-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` )
    REFERENCES `impact`.`spadstown` (`SYS_COD` , `PLOT_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`plotpaddoks`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotpaddoks` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `PORCTOT` DECIMAL(3,0) NULL DEFAULT 0 ,
  `LANDCOVER` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`) ,
  INDEX `SYSIDX015` (`SYS_COD` ASC, `PLOT_COD` ASC) ,
  CONSTRAINT `fk_91e7eec6-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` )
    REFERENCES `impact`.`spadstown` (`SYS_COD` , `PLOT_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`plotpaddspe`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotpaddspe` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `CUTCARRY` CHAR(1) NULL DEFAULT 'N' ,
  `NSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `landcover` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`) ,
  INDEX `SYSIDX017` (`CROP_COD` ASC) ,
  INDEX `SYSIDX016` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC) ,
  CONSTRAINT `fk_91e87a4e-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROP_COD` )
    REFERENCES `impact`.`pastures` (`CROP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_91eadcd0-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` )
    REFERENCES `impact`.`plotpaddoks` (`SYS_COD` , `PLOT_COD` , `NUMPADD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`plotpaddspedt`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotpaddspedt` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `CROPD_COD` CHAR(3) NOT NULL ,
  `LANDCOVER` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `CROPD_COD`) ,
  INDEX `SYSIDX019` (`CROPD_COD` ASC) ,
  INDEX `SYSIDX018` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC) ,
  CONSTRAINT `fk_91e908ba-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`CROPD_COD` )
    REFERENCES `impact`.`pastures` (`CROP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_91eb88ce-bce1-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    REFERENCES `impact`.`plotpaddspe` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`cropman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`) ,
  INDEX `SYSIDX020` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC) ,
  INDEX `SYSIDX021` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_3df4cb58-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    REFERENCES `impact`.`owneduse` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3df553c0-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`periods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`periods` (
  `period_id` CHAR(2) NOT NULL ,
  `period_des` VARCHAR(60) NULL ,
  `period_jan` CHAR(1) NULL DEFAULT 'F' ,
  `period_feb` CHAR(1) NULL DEFAULT 'F' ,
  `period_mar` CHAR(1) NULL DEFAULT 'F' ,
  `period_apr` CHAR(1) NULL DEFAULT 'F' ,
  `period_may` CHAR(1) NULL DEFAULT 'F' ,
  `period_jun` CHAR(1) NULL DEFAULT 'F' ,
  `period_jul` CHAR(1) NULL DEFAULT 'F' ,
  `period_aug` CHAR(1) NULL DEFAULT 'F' ,
  `period_sep` CHAR(1) NULL DEFAULT 'F' ,
  `period_oct` CHAR(1) NULL DEFAULT 'F' ,
  `period_nov` CHAR(1) NULL DEFAULT 'F' ,
  `period_dec` CHAR(1) NULL DEFAULT 'F' ,
  PRIMARY KEY (`period_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`cropmantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropmantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PLANDENS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `NMONTH`) ,
  INDEX `SYSIDX022` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX023` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX191` (`NMONTH` ASC) ,
  CONSTRAINT `fk_3df5da52-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`cropman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3df7044a-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_cropmantim_periods1`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`croplabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`croplabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX027` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX028` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_3df79fae-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`cropmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3df84800-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`cropmanfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropmanfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `NMONTH`, `FERTYP_COD`) ,
  INDEX `SYSIDX024` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX025` (`FERTYP_COD` ASC) ,
  INDEX `SYSIDX026` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  CONSTRAINT `fk_3df8edbe-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`cropmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3df9a02e-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfa4a06-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`cropmanprchfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropmanprchfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FERTYP_COD`) ,
  INDEX `SYSIDX029` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX030` (`FERTYP_COD` ASC) ,
  CONSTRAINT `fk_3dfaff5a-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`cropman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfbbdd2-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheap`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheap` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `HEAP_TYP` CHAR(3) NULL ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYASS` DECIMAL(12,3) NULL ,
  `PERCLOSS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `AP` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `GP` CHAR(1) NULL DEFAULT 'F' ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORAP` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORGP` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`) ,
  INDEX `SYSIDX149` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_42404990-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`cropmanheap`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropmanheap` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `QTYAPP` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `NMONTH`, `HEAP_COD`) ,
  INDEX `SYSIDX157` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX158` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX159` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  CONSTRAINT `fk_3dfc790c-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`cropmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfd4490-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfe0a56-bce2-11e0-a5d9-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpprodman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(12,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`) ,
  INDEX `SYSIDX031` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX032` (`CROP_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_44bdd730-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`cropman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_44be5f16-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`PROD_COD` , `CROP_COD` )
    REFERENCES `impact`.`cropprods` (`PROD_COD` , `CROP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpprodmantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodmantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `ST` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORST` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `NMONTH`) ,
  INDEX `SYSIDX033` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX034` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX192` (`NMONTH` ASC) ,
  CONSTRAINT `fk_44bf621c-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_44c1c25a-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`crpprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_crpprodmantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpprodlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX035` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX036` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_44bff0a6-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_44c274f2-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `NMONTH` )
    REFERENCES `impact`.`crpprodmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpprodmansold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodmansold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `MARK_COD`) ,
  INDEX `SYSIDX033` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX034` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_44c0841c-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`crpprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_44c1269c-bce3-11e0-a317-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpforaman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforaman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(12,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`) ,
  INDEX `SYSIDX037` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX038` (`CROP_COD` ASC, `FORA_COD` ASC) ,
  CONSTRAINT `fk_322bba82-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`cropman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_322c6d1a-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`FORA_COD` , `CROP_COD` )
    REFERENCES `impact`.`cropfora` (`FORA_COD` , `CROP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpforamansold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforamansold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `MARK_COD`) ,
  INDEX `SYSIDX039` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX040` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_322d0f86-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`crpforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_322da694-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpforamantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforamantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `ST` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORST` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `NMONTH`) ,
  INDEX `SYSIDX041` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX042` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX195` (`NMONTH` ASC) ,
  CONSTRAINT `fk_322e3410-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`crpforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_322ed352-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_crpforamantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpforalabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforalabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX043` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX044` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_322f701e-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `NMONTH` )
    REFERENCES `impact`.`crpforamantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_32302482-bce4-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`) ,
  INDEX `SYSIDX045` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC) ,
  INDEX `SYSIDX046` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_52042e10-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` )
    REFERENCES `impact`.`ownusedv` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_5204c096-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegmantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegmantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PLANDENS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `NMONTH`) ,
  INDEX `SYSIDX046` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX047` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX199` (`NMONTH` ASC) ,
  CONSTRAINT `fk_5205513c-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    REFERENCES `impact`.`vegman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_5205f4de-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_vegmantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`veglabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`veglabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX051` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX052` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_520691fa-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`vegmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_52073ee8-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegmanfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegmanfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `NMONTH`, `FERTYP_COD`) ,
  INDEX `SYSIDX048` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX049` (`FERTYP_COD` ASC) ,
  INDEX `SYSIDX050` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  CONSTRAINT `fk_5207e4e2-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`vegmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_52089d4c-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_52094b84-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegmanprchfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegmanprchfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FERTYP_COD`) ,
  INDEX `SYSIDX053` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX054` (`FERTYP_COD` ASC) ,
  CONSTRAINT `fk_520a0204-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    REFERENCES `impact`.`vegman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_520ac89c-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegmanheap`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegmanheap` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `QTYAPP` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `NMONTH`, `HEAP_COD`) ,
  INDEX `SYSIDX160` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX161` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX162` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  CONSTRAINT `fk_520b8c82-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`vegmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_520c5e14-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_520d2dbc-bce5-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegprodman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(12,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`) ,
  INDEX `SYSIDX055` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX056` (`VEG_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_cce75930-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    REFERENCES `impact`.`vegman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_cce7e5bc-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`VEG_COD` , `PROD_COD` )
    REFERENCES `impact`.`cropprods` (`CROP_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegprodmantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodmantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `ST` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORST` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`, `NMONTH`) ,
  INDEX `SYSIDX059` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX060` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX201` (`NMONTH` ASC) ,
  CONSTRAINT `fk_cce86a32-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`vegprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_cce900be-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_vegprodmantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegprodlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX061` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX062` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_cce990ec-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` , `NMONTH` )
    REFERENCES `impact`.`vegprodmantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_ccea3402-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegprodmansold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodmansold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`, `MARK_COD`) ,
  INDEX `SYSIDX057` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX058` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_cceacfc0-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`vegprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_cceb82b2-bce6-11e0-a317-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegforaman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforaman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(12,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`) ,
  INDEX `SYSIDX063` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX064` (`VEG_COD` ASC, `FORA_COD` ASC) ,
  CONSTRAINT `fk_9ad2a85e-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    REFERENCES `impact`.`vegman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_9ad2dbee-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`CROP_COD` , `FORA_COD` )
    REFERENCES `impact`.`cropfora` (`CROP_COD` , `FORA_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegforamantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforamantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `ST` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORST` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`, `NMONTH`) ,
  INDEX `SYSIDX067` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX068` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX204` (`NMONTH` ASC) ,
  CONSTRAINT `fk_9ad3628a-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`vegforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_9ad39b42-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_vegforamantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegforalabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforalabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX069` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX070` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_9ad3c702-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` , `NMONTH` )
    REFERENCES `impact`.`vegforamantim` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_9ad4005a-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegforamansold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforamansold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`, `MARK_COD`) ,
  INDEX `SYSIDX065` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX066` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_9ad30588-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`vegforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_9ad33a1c-bce7-11e0-a317-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`) ,
  INDEX `SYSIDX071` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC) ,
  INDEX `SYSIDX072` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_334977de-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    REFERENCES `impact`.`plotpaddspe` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_334a053c-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassmantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassmantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PLANDENS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `NMONTH`) ,
  INDEX `SYSIDX073` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX074` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX207` (`NMONTH` ASC) ,
  CONSTRAINT `fk_334a93d0-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`grassman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_334f51d6-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_grassmantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grasslabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grasslabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX080` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX081` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_334b315a-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`grassmantim` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_334bd74a-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassmanfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassmanfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `NMONTH`, `FERTYP_COD`) ,
  INDEX `SYSIDX075` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX076` (`FERTYP_COD` ASC) ,
  INDEX `SYSIDX077` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  CONSTRAINT `fk_334c80b4-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`grassmantim` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_334d32b6-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_33500fc2-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassmanprchfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassmanprchfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FERTYP_COD`) ,
  INDEX `SYSIDX078` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX079` (`FERTYP_COD` ASC) ,
  CONSTRAINT `fk_334ddf2c-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`grassman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_334e9ba6-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassmanheap`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassmanheap` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `QTYAPP` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `NMONTH`, `HEAP_COD`) ,
  INDEX `SYSIDX163` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX164` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX165` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  CONSTRAINT `fk_3350d272-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`grassmantim` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3351a292-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_33527082-bce8-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassprodman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NDF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(12,3) NULL DEFAULT 0 ,
  `ANENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`) ,
  INDEX `SYSIDX082` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX083` (`CROP_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_e2e10f26-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`grassman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e2e1bb1a-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`CROP_COD` , `PROD_COD` )
    REFERENCES `impact`.`pastprods` (`CROP_COD` , `PROD_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassprodmantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodmantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `ST` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORST` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `NMONTH`) ,
  INDEX `SYSIDX086` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX087` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX210` (`NMONTH` ASC) ,
  CONSTRAINT `fk_e2e27c58-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`grassprodman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e2e34ffc-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_grassprodmantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassprodlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX088` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX089` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_e2e41e3c-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `NMONTH` )
    REFERENCES `impact`.`grassprodmantim` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e2e50928-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassprodmansold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodmansold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `MARK_COD`) ,
  INDEX `SYSIDX084` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX085` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_e2e5e942-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`grassprodman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_e2e6d992-bce9-11e0-a317-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassforaman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforaman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL ,
  `PROTEIN` DECIMAL(12,3) NULL ,
  `NSEA1` DECIMAL(12,3) NULL ,
  `NSEA2` DECIMAL(12,3) NULL ,
  `NSEA3` DECIMAL(12,3) NULL ,
  `NSEA4` DECIMAL(12,3) NULL ,
  `PSEA1` DECIMAL(12,3) NULL ,
  `PSEA2` DECIMAL(12,3) NULL ,
  `PSEA3` DECIMAL(12,3) NULL ,
  `PSEA4` DECIMAL(12,3) NULL ,
  `KSEA1` DECIMAL(12,3) NULL ,
  `KSEA2` DECIMAL(12,3) NULL ,
  `KSEA3` DECIMAL(12,3) NULL ,
  `KSEA4` DECIMAL(12,3) NULL ,
  `CSEA1` DECIMAL(12,3) NULL ,
  `CSEA2` DECIMAL(12,3) NULL ,
  `CSEA3` DECIMAL(12,3) NULL ,
  `CSEA4` DECIMAL(12,3) NULL ,
  `DMSEA1` DECIMAL(12,3) NULL ,
  `DMSEA2` DECIMAL(12,3) NULL ,
  `DMSEA3` DECIMAL(12,3) NULL ,
  `DMSEA4` DECIMAL(12,3) NULL ,
  `DRYMATTER` DECIMAL(12,3) NULL ,
  `CPROTEIN` DECIMAL(12,3) NULL ,
  `NDF` DECIMAL(12,3) NULL ,
  `DIGESTI` DECIMAL(12,3) NULL ,
  `ANENERGY` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`) ,
  INDEX `SYSIDX090` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX091` (`CROP_COD` ASC, `FORA_COD` ASC) ,
  CONSTRAINT `fk_6ecf545c-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`grassman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_6ecf860c-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`CROP_COD` , `FORA_COD` )
    REFERENCES `impact`.`pastfora` (`CROP_COD` , `FORA_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassforamantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforamantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `ST` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORST` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `NMONTH`) ,
  INDEX `SYSIDX094` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX095` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX213` (`NMONTH` ASC) ,
  CONSTRAINT `fk_6ed016c6-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`grassforaman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_6ed04f42-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_grassforamantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassforalabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforalabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX096` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX098` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_6ed07f1c-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `NMONTH` )
    REFERENCES `impact`.`grassforamantim` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_6ed0bd38-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassforamansold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforamansold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `MARK_COD`) ,
  INDEX `SYSIDX092` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX093` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_6ecfb2c6-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`grassforaman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_6ecfeb7e-bcea-11e0-a317-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`feedgroups`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`feedgroups` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `GROUP_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `GROUP_TYP` CHAR(3) NULL DEFAULT 'ONE' ,
  `GROUP_CRIE` CHAR(3) NULL DEFAULT 'NON' ,
  `NUMANIM` DECIMAL(7,0) NULL DEFAULT 0 ,
  `INIWEIGHT` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FINWEIGHT` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FROMDAYS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `TODAYS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FROMPARTOS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `TOPARTOS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FROMMONTHS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `TOMONTHS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `FROMKILOS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `TOKILOS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX101` (`SYS_COD` ASC, `LIVE_ICOD` ASC) ,
  CONSTRAINT `fk_3994c7c6-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` )
    REFERENCES `impact`.`lvstosys` (`SYS_COD` , `LIVE_ICOD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livegroups`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livegroups` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `HERD_COD` CHAR(2) NOT NULL ,
  `NUMANIM` DECIMAL(7,0) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `HERD_COD`) ,
  INDEX `SYSIDX104` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_399baa0a-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`liveavailres`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`liveavailres` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `FEED_COD` CHAR(3) NOT NULL ,
  `COLLECTED` CHAR(1) NULL DEFAULT 'N' ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CPROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DETERFIBRE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DIGESTI` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `FEED_COD`) ,
  INDEX `SYSIDX099` (`SYS_COD` ASC) ,
  INDEX `SYSIDX100` (`FEED_COD` ASC) ,
  CONSTRAINT `fk_3995a826-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3996875a-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`FEED_COD` )
    REFERENCES `impact`.`restypes` (`RES_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`stallfeeprch`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`stallfeeprch` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `FEED_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `FEED_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX102` (`SYS_COD` ASC, `FEED_COD` ASC) ,
  INDEX `SYSIDX103` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_39977278-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `FEED_COD` )
    REFERENCES `impact`.`liveavailres` (`SYS_COD` , `FEED_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_39987088-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grazingman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grazingman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `MLIVE_COD` CHAR(3) NULL ,
  `MPROD_COD` CHAR(3) NULL ,
  `NSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `KSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA1` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA2` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA3` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DMSEA4` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX105` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC) ,
  INDEX `SYSIDX106` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  INDEX `SYSIDX170` (`MLIVE_COD` ASC, `MPROD_COD` ASC) ,
  CONSTRAINT `fk_39997924-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    REFERENCES `impact`.`plotpaddspe` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_399a8ba2-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_399ccfe8-bceb-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`MLIVE_COD` , `MPROD_COD` )
    REFERENCES `impact`.`lvproducts` (`LIVE_COD` , `PROD_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpprodmanfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodmanfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX107` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX108` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_11fb077e-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`crpprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_11fbb070-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`crpforamanfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforamanfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX109` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX110` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_11fc8fea-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`crpforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_11fd60d2-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegprodmanfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodmanfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX111` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX112` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_11fe14d2-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`vegprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_11fedb2e-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`vegforamanfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforamanfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX113` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX114` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_11ff9578-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`vegforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_12006d90-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassprodmanfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodmanfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX115` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX116` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_120155fc-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`grassprodman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_12022c20-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`grassforamanfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforamanfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX117` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  INDEX `SYSIDX118` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_12030438-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`grassforaman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_1203ea6a-bcec-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`liveman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`liveman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `ACTY_COD`) ,
  INDEX `SYSIDX119` (`SYS_COD` ASC, `LIVE_ICOD` ASC) ,
  INDEX `SYSIDX120` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_29cb5696-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` )
    REFERENCES `impact`.`lvstosys` (`SYS_COD` , `LIVE_ICOD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_29cbf222-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livemantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livemantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `ACTY_COD`, `NMONTH`) ,
  INDEX `SYSIDX121` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX227` (`NMONTH` ASC) ,
  CONSTRAINT `fk_29ce0558-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `ACTY_COD` )
    REFERENCES `impact`.`liveman` (`SYS_COD` , `LIVE_ICOD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_livemantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livelabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livelabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `ACTY_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX122` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX123` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_29cc7a26-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_29ceeee6-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`livemantim` (`SYS_COD` , `LIVE_ICOD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`otherprod`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherprod` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DESC` VARCHAR(30) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `PROD_COD`) ,
  INDEX `SYSIDX124` (`SYS_COD` ASC, `LIVE_ICOD` ASC) ,
  CONSTRAINT `fk_29cd31a0-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` )
    REFERENCES `impact`.`lvstosys` (`SYS_COD` , `LIVE_ICOD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livemangrp`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livemangrp` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `ACTY_COD`) ,
  INDEX `SYSIDX125` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  INDEX `SYSIDX126` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_29d0895e-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_29d1445c-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livemangrptim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livemangrptim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `ACTY_COD`, `NMONTH`) ,
  INDEX `SYSIDX127` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX228` (`NMONTH` ASC) ,
  CONSTRAINT `fk_29d207b6-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`livemangrp` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_livemangrptim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livemangrplabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livemangrplabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL ,
  `HOURSDAY` DECIMAL(12,3) NULL ,
  `PPERHOUR` DECIMAL(6,2) NULL ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `ACTY_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX128` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX129` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_29d2d448-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`livemangrptim` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_29d3b700-bced-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`farmassets`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`farmassets` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DES` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `TOTVALUE` DECIMAL(11,3) NULL DEFAULT 0 ,
  `ANUALDEP` DECIMAL(11,3) NULL DEFAULT 0 ,
  `PRCHYEAR` DECIMAL(11,3) NULL DEFAULT 0 ,
  `DISPVALUE` DECIMAL(11,3) NULL DEFAULT 0 ,
  `ANUMANT` DECIMAL(11,3) NULL DEFAULT 0 ,
  `NUMOWNED` DECIMAL(11,3) NULL DEFAULT 0 ,
  `NOWNMALE` DECIMAL(11,3) NULL DEFAULT 0 ,
  `NOWNFEMALE` DECIMAL(11,3) NULL DEFAULT 0 ,
  `NOWNBOTH` DECIMAL(11,3) NULL DEFAULT 0 ,
  `NOWNOTHERS` DECIMAL(11,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`) ,
  INDEX `SYSIDX140` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_4436dbf6-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`otherhhcp`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherhhcp` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DESC` VARCHAR(30) NULL DEFAULT 'Without description' ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`) ,
  INDEX `SYSIDX141` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_4437140e-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`otherhhexp`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherhhexp` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DESC` VARCHAR(30) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`) ,
  INDEX `SYSIDX142` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_44374c1c-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`otherincome`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherincome` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`) ,
  INDEX `SYSIDX143` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_44378808-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`otherman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `ACTY_COD`) ,
  INDEX `SYSIDX135` (`SYS_COD` ASC) ,
  INDEX `SYSIDX136` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_44360668-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_44363a70-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`othermantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`othermantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `ACTY_COD`, `NMONTH`) ,
  INDEX `SYSIDX137` (`SYS_COD` ASC, `ACTY_COD` ASC) ,
  INDEX `SYSIDX229` (`NMONTH` ASC) ,
  CONSTRAINT `fk_44366db0-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `ACTY_COD` )
    REFERENCES `impact`.`otherman` (`SYS_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_othermantim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`otherlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `ACTY_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX138` (`SYS_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX139` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_4436a456-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_4437c516-bcf5-11e0-9e78-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`othermantim` (`SYS_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livehvst`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvst` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`) ,
  INDEX `SYSIDX130` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  INDEX `SYSIDX131` (`LIVE_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_ee3ece80-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_ee3f5dd2-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`LIVE_COD` , `PROD_COD` )
    REFERENCES `impact`.`lvproducts` (`LIVE_COD` , `PROD_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livehvsttim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvsttim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `NMONTH`) ,
  INDEX `SYSIDX132` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `LIVE_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX233` (`NMONTH` ASC) ,
  CONSTRAINT `fk_ee3fe626-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    REFERENCES `impact`.`livehvst` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_livehvsttim_periods`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livehvstlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX133` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `LIVE_COD` ASC, `PROD_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX134` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_ee4080b8-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `NMONTH` )
    REFERENCES `impact`.`livehvsttim` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_ee4120e0-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livehvstsold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstsold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `MARK_COD`) ,
  INDEX `SYSIDX144` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `LIVE_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX145` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_ee41c09a-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    REFERENCES `impact`.`livehvst` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_ee426bb2-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livehvstcat`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstcat` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `HERD_COD` CHAR(2) NOT NULL ,
  `ENERGY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PROTEIN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `HERD_COD`) ,
  CONSTRAINT `fk_livehvstcat_livehvst1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    REFERENCES `impact`.`livehvst` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livehvsttimcat`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvsttimcat` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `HERD_COD` CHAR(2) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `YIELD` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` CHAR(1) NULL DEFAULT 'F' ,
  `FS` CHAR(1) NULL DEFAULT 'F' ,
  `MM` CHAR(1) NULL DEFAULT 'F' ,
  `LF` CHAR(1) NULL DEFAULT 'F' ,
  `LE` CHAR(1) NULL DEFAULT 'F' ,
  `OT` CHAR(1) NULL DEFAULT 'F' ,
  `RP` CHAR(1) NULL DEFAULT 'F' ,
  `DP` CHAR(1) NULL DEFAULT 'F' ,
  `PORHC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORFS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORMM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORLE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POROT` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORRP` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PORDP` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `HERD_COD`, `NMONTH`) ,
  INDEX `SYSIDX146` (`NMONTH` ASC) ,
  CONSTRAINT `fk_livehvsttimcat_livehvstcat1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` )
    REFERENCES `impact`.`livehvstcat` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livehvsttimcat_periods1`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livehvstsoldcat`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstsoldcat` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `HERD_COD` CHAR(2) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `HERD_COD`, `MARK_COD`) ,
  INDEX `SYSIDX147` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `LIVE_COD` ASC, `PROD_COD` ASC, `HERD_COD` ASC) ,
  INDEX `SYSIDX148` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_ee43d100-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` )
    REFERENCES `impact`.`livehvstcat` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_ee4490b8-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`livehvstfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `FLIVE_ICOD` CHAR(3) NOT NULL ,
  `FGROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `FLIVE_ICOD`, `FGROUP_COD`) ,
  INDEX `SYSIDX166` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `LIVE_COD` ASC, `PROD_COD` ASC) ,
  INDEX `SYSIDX167` (`SYS_COD` ASC, `FLIVE_ICOD` ASC, `FGROUP_COD` ASC) ,
  CONSTRAINT `fk_ee454b02-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    REFERENCES `impact`.`livehvst` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_ee4617da-bcf8-11e0-948f-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `FLIVE_ICOD` , `FGROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheaplvprod`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheaplvprod` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`) ,
  INDEX `SYSIDX150` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX151` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC, `LIVE_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_4240db80-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_42417cf2-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    REFERENCES `impact`.`livehvst` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapprods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapprods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `PROD_DES` VARCHAR(60) NULL DEFAULT 'Without description' ,
  `COLLECTED` CHAR(1) NULL DEFAULT 'N' ,
  `NITROGEN` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PHOSPHORUS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `POTASSIUM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CARBON` DECIMAL(12,3) NULL DEFAULT 0 ,
  `DRYMATTER` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`) ,
  INDEX `SYSIDX152` (`SYS_COD` ASC) ,
  CONSTRAINT `fk_42422850-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapaddprods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapaddprods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `PROD_COD`) ,
  INDEX `SYSIDX153` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX154` (`SYS_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_4242d584-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_42438ace-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PROD_COD` )
    REFERENCES `impact`.`mnrheapprods` (`SYS_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapsold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapsold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `DESRELAT` VARCHAR(20) NULL DEFAULT 'Without description' ,
  `INDRELAT` CHAR(1) NULL DEFAULT 'F' ,
  `QTYRELAT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `MARK_COD`) ,
  INDEX `SYSIDX155` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX156` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_42444a54-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_42450cc8-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapfeed`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapfeed` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `LIVE_ICOD`, `GROUP_COD`) ,
  INDEX `SYSIDX168` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX169` (`SYS_COD` ASC, `LIVE_ICOD` ASC, `GROUP_COD` ASC) ,
  CONSTRAINT `fk_4245d9a0-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_4246acfe-bcfa-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapcrpprod`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapcrpprod` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`) ,
  INDEX `SYSIDX170` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX171` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_81de4dee-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_81df0784-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`crpprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapcrpfora`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapcrpfora` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`) ,
  INDEX `SYSIDX172` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX173` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  CONSTRAINT `fk_81dfd6dc-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_81e0a60c-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`crpforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapgrassprod`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapgrassprod` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`) ,
  INDEX `SYSIDX174` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX175` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_81e18ba8-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_81e26a96-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`grassprodman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapgrassfora`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapgrassfora` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`) ,
  INDEX `SYSIDX176` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX177` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMPADD` ASC, `CROP_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  CONSTRAINT `fk_81e35b04-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_81e44a64-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`grassforaman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapvegprod`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapvegprod` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`) ,
  INDEX `SYSIDX178` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX179` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_81e549e6-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_81e64d8c-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`vegprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapvegfora`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapvegfora` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `QTYASS` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`) ,
  INDEX `SYSIDX180` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX181` (`SYS_COD` ASC, `PLOT_COD` ASC, `NUMSEASON` ASC, `CROP_COD` ASC, `VEG_COD` ASC, `ACTY_COD` ASC, `FORA_COD` ASC) ,
  CONSTRAINT `fk_81e7808a-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_81e8dec6-bcfb-11e0-b883-58946b4c242c`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`vegforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`nfoutcrops`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`nfoutcrops` (
  `sys_cod` VARCHAR(27) NULL ,
  `plot_cod` CHAR(3) NULL ,
  `crop_cod` CHAR(3) NULL ,
  `iscrop` CHAR(1) NULL DEFAULT 'Y' ,
  `outtype` CHAR(2) NULL DEFAULT 'UN' ,
  `nitrogen` DECIMAL(12,3) NULL DEFAULT 0 ,
  `phosphorus` DECIMAL(12,3) NULL DEFAULT 0 ,
  `potassium` DECIMAL(12,3) NULL DEFAULT 0 ,
  `carbon` DECIMAL(12,3) NULL DEFAULT 0 )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`nfoutlives`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`nfoutlives` (
  `sys_cod` VARCHAR(27) NULL ,
  `live_icod` CHAR(3) NULL ,
  `live_cod` CHAR(3) NULL ,
  `prod_cod` CHAR(3) NULL ,
  `outtype` CHAR(2) NULL DEFAULT 'UN' ,
  `anweight` DECIMAL(12,3) NULL DEFAULT 0 ,
  `nitrogen` DECIMAL(12,3) NULL DEFAULT 0 ,
  `phosphorus` DECIMAL(12,3) NULL DEFAULT 0 ,
  `potassium` DECIMAL(12,3) NULL DEFAULT 0 ,
  `carbon` DECIMAL(12,3) NULL DEFAULT 0 )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`nfinscrops`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`nfinscrops` (
  `sys_cod` VARCHAR(27) NULL ,
  `plot_cod` CHAR(3) NULL ,
  `crop_cod` CHAR(3) NULL ,
  `iscrop` CHAR(1) NULL DEFAULT 'Y' ,
  `inputsrc` CHAR(2) NULL DEFAULT 'UN' ,
  `inputtyp` CHAR(1) NULL DEFAULT 'U' ,
  `nitrogen` DECIMAL(12,3) NULL DEFAULT 0 ,
  `phosphorus` DECIMAL(12,3) NULL DEFAULT 0 ,
  `potassium` DECIMAL(12,3) NULL DEFAULT 0 ,
  `carbon` DECIMAL(12,3) NULL DEFAULT 0 )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`nfinslives`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`nfinslives` (
  `sys_cod` VARCHAR(27) NULL ,
  `live_icod` CHAR(3) NULL ,
  `live_cod` CHAR(3) NULL ,
  `inputsrc` CHAR(2) NULL DEFAULT 'UN' ,
  `inputtyp` CHAR(1) NULL DEFAULT 'U' ,
  `anweight` DECIMAL(12,3) NULL DEFAULT 0 ,
  `nitrogen` DECIMAL(12,3) NULL DEFAULT 0 ,
  `phosphorus` DECIMAL(12,3) NULL DEFAULT 0 ,
  `potassium` DECIMAL(12,3) NULL DEFAULT 0 ,
  `carbon` DECIMAL(12,3) NULL DEFAULT 0 )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`nfgrazinggrps`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`nfgrazinggrps` (
  `plot_cod` CHAR(3) NOT NULL ,
  `numpadd` CHAR(2) NOT NULL ,
  `crop_cod` CHAR(3) NOT NULL ,
  `live_icod` CHAR(3) NOT NULL ,
  `group_cod` CHAR(3) NOT NULL ,
  `nmonth` SMALLINT UNSIGNED NULL DEFAULT 0 ,
  `intake` DECIMAL(12,3) NULL DEFAULT 0 )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`nfinsheaps`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`nfinsheaps` (
  `sys_cod` VARCHAR(27) NULL ,
  `heap_cod` CHAR(3) NULL ,
  `inputsrc` CHAR(2) NULL DEFAULT 'UN' ,
  `inputtyp` CHAR(1) NULL DEFAULT 'U' ,
  `nitrogen` DECIMAL(12,3) NULL DEFAULT 0 ,
  `phosphorus` DECIMAL(12,3) NULL DEFAULT 0 ,
  `potassium` DECIMAL(12,3) NULL DEFAULT 0 ,
  `carbon` DECIMAL(12,3) NULL DEFAULT 0 )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`nfoutheaps`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`nfoutheaps` (
  `sys_cod` VARCHAR(27) NULL ,
  `heap_cod` CHAR(3) NULL ,
  `outtype` CHAR(2) NULL DEFAULT 'UN' ,
  `nitrogen` DECIMAL(12,3) NULL DEFAULT 0 ,
  `phosphorus` DECIMAL(12,3) NULL DEFAULT 0 ,
  `potassium` DECIMAL(12,3) NULL DEFAULT 0 ,
  `carbon` DECIMAL(12,3) NULL DEFAULT 0 )
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`project`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`project` (
  `project_id` INT NOT NULL AUTO_INCREMENT ,
  `project_des` VARCHAR(120) NULL ,
  `project_sdate` DATE NULL ,
  `project_edate` DATE NULL ,
  `project_mdata` BLOB NULL ,
  `project_language` CHAR(4) NULL ,
  `project_nperiods` INT NULL ,
  `project_perioddesc` VARCHAR(120) NULL ,
  PRIMARY KEY (`project_id`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`systemperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`systemperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `rain` DECIMAL(7,3) NULL DEFAULT 0 ,
  `avetemp` DECIMAL(7,3) NULL DEFAULT 0 ,
  `maxtemp` DECIMAL(7,3) NULL DEFAULT 0 ,
  `mintemp` DECIMAL(7,3) NULL DEFAULT 0 ,
  `season` CHAR(2) NULL DEFAULT 'DR' ,
  `rday` DECIMAL(6,2) NULL DEFAULT 0 ,
  `sun` CHAR(1) NULL DEFAULT '1' ,
  PRIMARY KEY (`SYS_COD`, `period_id`) ,
  INDEX `SYSIDX183` (`period_id` ASC) ,
  CONSTRAINT `fk_systemperiod_system1`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_systemperiod_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`labforceperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`labforceperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `wrk` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`SYS_COD`, `PERS_COD`, `period_id`) ,
  INDEX `SYSIDX184` (`period_id` ASC) ,
  CONSTRAINT `fk_table1_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_table1_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`lvstosysperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`lvstosysperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `part12` DECIMAL(7,0) NULL DEFAULT 0 ,
  `part23` DECIMAL(7,0) NULL DEFAULT 0 ,
  `parto3` DECIMAL(7,0) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `period_id`) ,
  INDEX `SYSIDX185` (`period_id` ASC) ,
  CONSTRAINT `fk_lvstosysperiods_lvstosys1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` )
    REFERENCES `impact`.`lvstosys` (`SYS_COD` , `LIVE_ICOD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_lvstosysperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotseasonsperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotseasonsperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `grw` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `period_id`) ,
  INDEX `SYSIDX186` (`period_id` ASC) ,
  CONSTRAINT `fk_plotseasonsperiods_plotseasons1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` )
    REFERENCES `impact`.`plotseasons` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotseasonsperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`owneduseperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`owneduseperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `crp` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `period_id`) ,
  INDEX `SYSIDX187` (`period_id` ASC) ,
  CONSTRAINT `fk_owneduseperiods_owneduse1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    REFERENCES `impact`.`owneduse` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_owneduseperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`ownusedvperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`ownusedvperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `harv` CHAR(1) NULL DEFAULT 'N' ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `period_id`) ,
  INDEX `SYSIDX188` (`period_id` ASC) ,
  CONSTRAINT `fk_ownusedvperiods_ownusedv1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` )
    REFERENCES `impact`.`ownusedv` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_ownusedvperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`cropmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `period_id`) ,
  INDEX `SYSIDX189` (`period_id` ASC) ,
  CONSTRAINT `fk_cropmanperiods_cropman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`cropman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_cropmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`cropmanprchfertperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropmanprchfertperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FERTYP_COD`, `period_id`) ,
  INDEX `SYSIDX190` (`period_id` ASC) ,
  CONSTRAINT `fk_cropmanprchfertperiods_cropmanprchfert1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FERTYP_COD` )
    REFERENCES `impact`.`cropmanprchfert` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FERTYP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_cropmanprchfertperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`crpprodmansoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodmansoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX193` (`period_id` ASC) ,
  CONSTRAINT `fk_crpprodmansoldperiods_crpprodmansold1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `MARK_COD` )
    REFERENCES `impact`.`crpprodmansold` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_crpprodmansoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`crpprodmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX194` (`period_id` ASC) ,
  CONSTRAINT `fk_crpprodmanperiods_crpprodman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`crpprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_crpprodmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`crpforamanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforamanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `period_id`) ,
  INDEX `SYSIDX196` (`period_id` ASC) ,
  CONSTRAINT `fk_crpforamanperiods_crpforaman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`crpforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_crpforamanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`crpforamansoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforamansoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX197` (`period_id` ASC) ,
  CONSTRAINT `fk_crpforamansoldperiods_crpforamansold1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `MARK_COD` )
    REFERENCES `impact`.`crpforamansold` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_crpforamansoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `period_id`) ,
  INDEX `SYSIDX198` (`period_id` ASC) ,
  CONSTRAINT `fk_vegmanperiods_vegman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    REFERENCES `impact`.`vegman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegmanprchfertperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegmanprchfertperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FERTYP_COD`, `period_id`) ,
  INDEX `SYSIDX200` (`period_id` ASC) ,
  CONSTRAINT `fk_vegmanprchfertperiods_vegmanprchfert1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FERTYP_COD` )
    REFERENCES `impact`.`vegmanprchfert` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FERTYP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegmanprchfertperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegprodmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX202` (`period_id` ASC) ,
  CONSTRAINT `fk_vegprodmanperiods_vegprodman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`vegprodman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegprodmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegprodmansoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodmansoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX203` (`period_id` ASC) ,
  CONSTRAINT `fk_vegprodmansoldperiods_vegprodmansold1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` , `MARK_COD` )
    REFERENCES `impact`.`vegprodmansold` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegprodmansoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegforamanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforamanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`, `period_id`) ,
  INDEX `SYSIDX205` (`period_id` ASC) ,
  CONSTRAINT `fk_vegforamanperiods_vegforaman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`vegforaman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegforamanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegforamansoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforamansoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX206` (`period_id` ASC) ,
  CONSTRAINT `fk_vegforamansoldperiods_vegforamansold1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` , `MARK_COD` )
    REFERENCES `impact`.`vegforamansold` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegforamansoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `period_id`) ,
  INDEX `SYSIDX208` (`period_id` ASC) ,
  CONSTRAINT `fk_grassmanperiods_grassman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`grassman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassmanprchfertperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassmanprchfertperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FERTYP_COD`, `period_id`) ,
  INDEX `SYSIDX209` (`period_id` ASC) ,
  CONSTRAINT `fk_grassmanprchfertperiods_grassmanprchfert1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FERTYP_COD` )
    REFERENCES `impact`.`grassmanprchfert` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FERTYP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassmanprchfertperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassprodmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX211` (`period_id` ASC) ,
  CONSTRAINT `fk_grassprodmanperiods_grassprodman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    REFERENCES `impact`.`grassprodman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassprodmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassprodmansoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodmansoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX212` (`period_id` ASC) ,
  CONSTRAINT `fk_grassprodmansoldperiods_grassprodmansold1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `MARK_COD` )
    REFERENCES `impact`.`grassprodmansold` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassprodmansoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassforamanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforamanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `period_id`) ,
  INDEX `SYSIDX214` (`period_id` ASC) ,
  CONSTRAINT `fk_grassforamanperiods_grassforaman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    REFERENCES `impact`.`grassforaman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassforamanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassforamansoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforamansoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX215` (`period_id` ASC) ,
  CONSTRAINT `fk_grassforamansoldperiods_grassforamansold1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `MARK_COD` )
    REFERENCES `impact`.`grassforamansold` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassforamansoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`liveavailresperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`liveavailresperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `FEED_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `ava` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `FEED_COD`, `period_id`) ,
  INDEX `SYSIDX216` (`period_id` ASC) ,
  CONSTRAINT `fk_liveavailresperiods_liveavailres1`
    FOREIGN KEY (`SYS_COD` , `FEED_COD` )
    REFERENCES `impact`.`liveavailres` (`SYS_COD` , `FEED_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_liveavailresperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`stallfeeprchperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`stallfeeprchperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `FEED_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `FEED_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX217` (`period_id` ASC) ,
  CONSTRAINT `fk_stallfeeprchperiods_stallfeeprch1`
    FOREIGN KEY (`SYS_COD` , `FEED_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`stallfeeprch` (`SYS_COD` , `FEED_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_stallfeeprchperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livegroupsperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livegroupsperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `HERD_COD` CHAR(2) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `na` DECIMAL(7,0) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `HERD_COD`, `period_id`) ,
  INDEX `SYSIDX218` (`period_id` ASC) ,
  CONSTRAINT `fk_livegroupsperiods_livegroups1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `HERD_COD` )
    REFERENCES `impact`.`livegroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `HERD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livegroupsperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grazingmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grazingmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `rot` DECIMAL(12,3) NULL DEFAULT 0 ,
  `ncd` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pia` DECIMAL(12,3) NULL DEFAULT 0 ,
  `mnr` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX219` (`period_id` ASC) ,
  CONSTRAINT `fk_grazingmanperiods_grazingman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`grazingman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grazingmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`crpprodmanfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpprodmanfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX220` (`period_id` ASC) ,
  CONSTRAINT `fk_crpprodmanfeedperiods_crpprodmanfeed1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`crpprodmanfeed` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_crpprodmanfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`crpforamanfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`crpforamanfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX221` (`period_id` ASC) ,
  CONSTRAINT `fk_crpforamanfeedperiods_crpforamanfeed1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`crpforamanfeed` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_crpforamanfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegprodmanfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegprodmanfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PROD_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX222` (`period_id` ASC) ,
  CONSTRAINT `fk_vegprodmanfeedperiods_vegprodmanfeed1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`vegprodmanfeed` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `PROD_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegprodmanfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegforamanfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegforamanfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `FORA_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX223` (`period_id` ASC) ,
  CONSTRAINT `fk_vegforamanfeedperiods_vegforamanfeed1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`vegforamanfeed` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` , `FORA_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegforamanfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassprodmanfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassprodmanfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PROD_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX224` (`period_id` ASC) ,
  CONSTRAINT `fk_grassprodmanfeedperiods_grassprodmanfeed1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`grassprodmanfeed` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `PROD_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassprodmanfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassforamanfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassforamanfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `FORA_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX225` (`period_id` ASC) ,
  CONSTRAINT `fk_grassforamanfeedperiods_grassforamanfeed1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`grassforamanfeed` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` , `FORA_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassforamanfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`otherprodperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherprodperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tmp` CHAR(1) NULL DEFAULT 'N' ,
  `qtp` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX226` (`period_id` ASC) ,
  CONSTRAINT `fk_otherprodperiods_otherprod1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `PROD_COD` )
    REFERENCES `impact`.`otherprod` (`SYS_COD` , `LIVE_ICOD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_otherprodperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`otherhhcpperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherhhcpperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tmp` CHAR(1) NULL DEFAULT 'N' ,
  `qtp` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX230` (`period_id` ASC) ,
  CONSTRAINT `fk_otherhhcpperiods_otherhhcp1`
    FOREIGN KEY (`SYS_COD` , `PROD_COD` )
    REFERENCES `impact`.`otherhhcp` (`SYS_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_otherhhcpperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`otherhhexpperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherhhexpperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tmp` CHAR(1) NULL DEFAULT 'N' ,
  `qtp` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(13,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX231` (`period_id` ASC) ,
  CONSTRAINT `fk_otherhhexpperiods_otherhhexp1`
    FOREIGN KEY (`SYS_COD` , `PROD_COD` )
    REFERENCES `impact`.`otherhhexp` (`SYS_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_otherhhexpperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`otherincomeperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`otherincomeperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tmp` CHAR(1) NULL DEFAULT 'N' ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX232` (`period_id` ASC) ,
  CONSTRAINT `fk_otherincomeperiods_otherincome1`
    FOREIGN KEY (`SYS_COD` , `PROD_COD` )
    REFERENCES `impact`.`otherincome` (`SYS_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_otherincomeperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livehvstperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  `wei` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX234` (`period_id` ASC) ,
  CONSTRAINT `fk_livehvstperiods_livehvst1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    REFERENCES `impact`.`livehvst` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livehvstperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livehvstfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `FLIVE_ICOD` CHAR(3) NOT NULL ,
  `FGROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `FLIVE_ICOD`, `FGROUP_COD`, `period_id`) ,
  INDEX `SYSIDX235` (`period_id` ASC) ,
  CONSTRAINT `fk_livehvstfeedperiods_livehvstfeed1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `FLIVE_ICOD` , `FGROUP_COD` )
    REFERENCES `impact`.`livehvstfeed` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `FLIVE_ICOD` , `FGROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livehvstfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livehvstsoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstsoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  `wei` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX236` (`period_id` ASC) ,
  CONSTRAINT `fk_livehvstsoldperiods_livehvstsold1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `MARK_COD` )
    REFERENCES `impact`.`livehvstsold` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livehvstsoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livehvstsoldcatperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstsoldcatperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `HERD_COD` CHAR(2) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  `wei` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `HERD_COD`, `MARK_COD`, `period_id`) ,
  INDEX `SYSIDX237` (`period_id` ASC) ,
  CONSTRAINT `fk_livehvstsoldcatperiods_livehvstsoldcat1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` , `MARK_COD` )
    REFERENCES `impact`.`livehvstsoldcat` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livehvstsoldcatperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapfeedperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapfeedperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'F' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `HEAP_COD`, `LIVE_ICOD`, `GROUP_COD`, `period_id`) ,
  INDEX `SYSIDX238` (`period_id` ASC) ,
  CONSTRAINT `fk_mnrheapfeedperiods_mnrheapfeed1`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`mnrheapfeed` (`SYS_COD` , `HEAP_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_mnrheapfeedperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapprodsperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapprodsperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `ava` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PROD_COD`, `period_id`) ,
  INDEX `SYSIDX239` (`period_id` ASC) ,
  CONSTRAINT `fk_mnrheapprodsperiods_mnrheapprods1`
    FOREIGN KEY (`SYS_COD` , `PROD_COD` )
    REFERENCES `impact`.`mnrheapprods` (`SYS_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_mnrheapprodsperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`mnrheapsoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`mnrheapsoldperiods` (
  `mnrheapsold_SYS_COD` VARCHAR(27) NOT NULL ,
  `mnrheapsold_HEAP_COD` CHAR(3) NOT NULL ,
  `mnrheapsold_MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`mnrheapsold_SYS_COD`, `mnrheapsold_HEAP_COD`, `mnrheapsold_MARK_COD`, `period_id`) ,
  INDEX `SYSIDX240` (`period_id` ASC) ,
  CONSTRAINT `fk_mnrheapsoldperiods_mnrheapsold1`
    FOREIGN KEY (`mnrheapsold_SYS_COD` , `mnrheapsold_HEAP_COD` , `mnrheapsold_MARK_COD` )
    REFERENCES `impact`.`mnrheapsold` (`SYS_COD` , `HEAP_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_mnrheapsoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`owneduselabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`owneduselabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `PERS_COD`) ,
  INDEX `SYSIDX141` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_owneduselabour_owneduse1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    REFERENCES `impact`.`owneduse` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_owneduselabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotpaddspelabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotpaddspelabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `PERS_COD`) ,
  INDEX `SYSIDX242` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_plotpaddspelabour_plotpaddspe1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    REFERENCES `impact`.`plotpaddspe` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotpaddspelabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`ownusedvlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`ownusedvlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `PERS_COD`) ,
  INDEX `SYSIDX243` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_ownusedvlabour_ownusedv1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` )
    REFERENCES `impact`.`ownusedv` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_ownusedvlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`cropmanlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`cropmanlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `ACTY_COD`, `PERS_COD`) ,
  INDEX `SYSIDX244` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_cropmanlabour_cropman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`cropman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_cropmanlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`vegmanlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`vegmanlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMSEASON` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `VEG_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMSEASON`, `CROP_COD`, `VEG_COD`, `ACTY_COD`, `PERS_COD`) ,
  INDEX `SYSIDX245` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_vegmanlabour_vegman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    REFERENCES `impact`.`vegman` (`SYS_COD` , `PLOT_COD` , `NUMSEASON` , `CROP_COD` , `VEG_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_vegmanlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grassmanlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grassmanlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `NUMPADD` CHAR(2) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `NUMPADD`, `CROP_COD`, `ACTY_COD`, `PERS_COD`) ,
  INDEX `SYSIDX246` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_grassmanlabour_grassman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`grassman` (`SYS_COD` , `PLOT_COD` , `NUMPADD` , `CROP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grassmanlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`lvstosyslabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`lvstosyslabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `PERS_COD`) ,
  INDEX `SYSIDX247` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_lvstosyslabour_lvstosys1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` )
    REFERENCES `impact`.`lvstosys` (`SYS_COD` , `LIVE_ICOD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_lvstosyslabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`feedgroupslabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`feedgroupslabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `PERS_COD`) ,
  INDEX `SYSIDX248` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_feedgroupslabour_feedgroups1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    REFERENCES `impact`.`feedgroups` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_feedgroupslabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`lvstosysherdstr`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`lvstosysherdstr` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `herd_cod` CHAR(2) NOT NULL ,
  `NANIMAL` DECIMAL(7,0) NULL DEFAULT 0 ,
  `NANIMALAS` DECIMAL(7,0) NULL DEFAULT 0 ,
  `bw` DECIMAL(7,3) NULL DEFAULT 0 ,
  `perfertile` DECIMAL(7,3) NULL DEFAULT 0 ,
  `permilked` DECIMAL(7,3) NULL DEFAULT 0 ,
  `cvalue` DECIMAL(12,3) NULL DEFAULT 0 ,
  `mortality` DECIMAL(7,3) NULL DEFAULT 0 ,
  `numfertile` DECIMAL(7,0) NULL DEFAULT 0 ,
  `numferliteas` DECIMAL(7,0) NULL DEFAULT 0 ,
  `nummilked` DECIMAL(7,0) NULL DEFAULT 0 ,
  `nummilkedas` DECIMAL(7,0) NULL DEFAULT 0 ,
  `numdry` DECIMAL(7,0) NULL DEFAULT 0 ,
  `numdryas` DECIMAL(7,0) NULL DEFAULT 0 ,
  `numlactating` DECIMAL(7,0) NULL DEFAULT 0 ,
  `numlactatingas` DECIMAL(7,0) NULL DEFAULT 0 ,
  `TOTPART` DECIMAL(7,0) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `herd_cod`) ,
  CONSTRAINT `fk_lvstosysherdstr_lvstosys1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` )
    REFERENCES `impact`.`lvstosys` (`SYS_COD` , `LIVE_ICOD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livemanlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livemanlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `ACTY_COD`, `PERS_COD`) ,
  INDEX `SYSIDX249` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_livemanlabour_liveman1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `ACTY_COD` )
    REFERENCES `impact`.`liveman` (`SYS_COD` , `LIVE_ICOD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livemanlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livemangrplabourB`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livemangrplabourB` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `ACTY_COD`, `PERS_COD`) ,
  INDEX `SYSIDX250` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_table1_livemangrp1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `ACTY_COD` )
    REFERENCES `impact`.`livemangrp` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livemangrplabourB_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`othermanlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`othermanlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `ACTY_COD`, `PERS_COD`) ,
  INDEX `SYSIDX250` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_othermanlabour_otherman1`
    FOREIGN KEY (`SYS_COD` , `ACTY_COD` )
    REFERENCES `impact`.`otherman` (`SYS_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_othermanlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`moduletree`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`moduletree` (
  `module_id` CHAR(3) NOT NULL ,
  `module_library` VARCHAR(254) NULL ,
  `module_plugin` VARCHAR(120) NULL ,
  `module_pluginDesc` VARCHAR(254) NULL ,
  `module_order` INT(11)  NULL ,
  `parent_module` CHAR(3) NULL DEFAULT NULL ,
  PRIMARY KEY (`module_id`) ,
  INDEX `MNTIDXMODULES` (`parent_module` ASC) ,
  CONSTRAINT `fk_moduletree_moduletree1`
    FOREIGN KEY (`parent_module` )
    REFERENCES `impact`.`moduletree` (`module_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`sysmodulenotes`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`sysmodulenotes` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `library` VARCHAR(45) NOT NULL ,
  `plugin` VARCHAR(45) NOT NULL ,
  `notes` BLOB NULL ,
  PRIMARY KEY (`SYS_COD`, `library`, `plugin`) ,
  CONSTRAINT `fk_sysmodulenotes_system1`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`livehvstcatperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`livehvstcatperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `LIVE_ICOD` CHAR(3) NOT NULL ,
  `GROUP_COD` CHAR(3) NOT NULL ,
  `LIVE_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `HERD_COD` CHAR(2) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  `wei` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `LIVE_ICOD`, `GROUP_COD`, `LIVE_COD`, `PROD_COD`, `HERD_COD`, `period_id`) ,
  INDEX `SYSIDX251` (`period_id` ASC) ,
  CONSTRAINT `fk_livehvstcatperiods_livehvstcat1`
    FOREIGN KEY (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` )
    REFERENCES `impact`.`livehvstcat` (`SYS_COD` , `LIVE_ICOD` , `GROUP_COD` , `LIVE_COD` , `PROD_COD` , `HERD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_livehvstcatperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotman`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotman` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `mandays` DECIMAL(12,3) NULL ,
  `pricemanday` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`) ,
  INDEX `SYSIDX252` (`ACTY_COD` ASC) ,
  CONSTRAINT `fk_plotman_spadstown1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` )
    REFERENCES `impact`.`spadstown` (`SYS_COD` , `PLOT_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotman_activity1`
    FOREIGN KEY (`ACTY_COD` )
    REFERENCES `impact`.`activity` (`ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotmanperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotmanperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `TIM` CHAR(1) NULL DEFAULT 'N' ,
  `QTY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRI` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `period_id`) ,
  INDEX `SYSIDX253` (`period_id` ASC) ,
  CONSTRAINT `fk_plotmanperiods_plotman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    REFERENCES `impact`.`plotman` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotmanperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotmantim`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotmantim` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PLANDENS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `CHECKED` CHAR(1) NULL DEFAULT 'N' ,
  `mandays` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pricemanday` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `NMONTH`) ,
  INDEX `SYSIDX023` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX191` (`NMONTH` ASC) ,
  INDEX `SYSIDX254` (`SYS_COD` ASC, `PLOT_COD` ASC, `ACTY_COD` ASC) ,
  CONSTRAINT `fk_3df7044a-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_plotmantim_periods10`
    FOREIGN KEY (`NMONTH` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE RESTRICT
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotman_plotmantim`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    REFERENCES `impact`.`plotman` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`plotlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,2) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `NMONTH`, `PERS_COD`) ,
  INDEX `SYSIDX255` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_plotlabour_plotmantim1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`plotmantim` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotmanfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotmanfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `PRCH` CHAR(1) NULL DEFAULT 'F' ,
  `FARM` CHAR(1) NULL DEFAULT 'F' ,
  `EXCH` CHAR(1) NULL DEFAULT 'F' ,
  `QTYPRCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYFARM` DECIMAL(12,3) NULL DEFAULT 0 ,
  `QTYEXCH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHINE` CHAR(1) NULL DEFAULT 'F' ,
  `FREQMACH` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRICE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `MACHDESC` VARCHAR(120) NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `NMONTH`, `FERTYP_COD`) ,
  INDEX `SYSIDX025` (`FERTYP_COD` ASC) ,
  INDEX `SYSIDX026` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  INDEX `SYSIDX257` (`SYS_COD` ASC, `PLOT_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  CONSTRAINT `fk_3df9a02e-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfa4a06-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE SET NULL,
  CONSTRAINT `fk_plotmanfert_plotmantim1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`plotmantim` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`plotmanprchfert`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotmanprchfert` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `FERTYP_COD`) ,
  INDEX `SYSIDX256` (`FERTYP_COD` ASC) ,
  CONSTRAINT `fk_plotmanprchfert_plotman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    REFERENCES `impact`.`plotman` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotmanprchfert_ferttype1`
    FOREIGN KEY (`FERTYP_COD` )
    REFERENCES `impact`.`ferttype` (`FERTYP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotmanprchfertperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotmanprchfertperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `FERTYP_COD` VARCHAR(5) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `tim` CHAR(1) NULL DEFAULT 'N' ,
  `QTY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PRI` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `FERTYP_COD`, `period_id`) ,
  INDEX `SYSIDX258` (`period_id` ASC) ,
  CONSTRAINT `fk_plotmanprchfertperiods_plotmanprchfert1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `FERTYP_COD` )
    REFERENCES `impact`.`plotmanprchfert` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `FERTYP_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotmanprchfertperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotmanlabour`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotmanlabour` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `PERS_COD` VARCHAR(5) NOT NULL ,
  `FREQUENCY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HOURSDAY` DECIMAL(12,3) NULL DEFAULT 0 ,
  `PPERHOUR` DECIMAL(6,2) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `PERS_COD`) ,
  INDEX `SYSIDX259` (`SYS_COD` ASC, `PERS_COD` ASC) ,
  CONSTRAINT `fk_plotmanlabour_plotman1`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    REFERENCES `impact`.`plotman` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_plotmanlabour_labforce1`
    FOREIGN KEY (`SYS_COD` , `PERS_COD` )
    REFERENCES `impact`.`labforce` (`SYS_COD` , `PERS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`plotmanheap`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`plotmanheap` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `PLOT_COD` CHAR(3) NOT NULL ,
  `ACTY_COD` CHAR(3) NOT NULL ,
  `NMONTH` CHAR(2) NOT NULL ,
  `HEAP_COD` CHAR(3) NOT NULL ,
  `ACTYM_COD` CHAR(3) NULL ,
  `MET_COD` VARCHAR(5) NULL ,
  `QTYAPP` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `PLOT_COD`, `ACTY_COD`, `NMONTH`, `HEAP_COD`) ,
  INDEX `SYSIDX160` (`SYS_COD` ASC, `PLOT_COD` ASC, `ACTY_COD` ASC, `NMONTH` ASC) ,
  INDEX `SYSIDX161` (`SYS_COD` ASC, `HEAP_COD` ASC) ,
  INDEX `SYSIDX162` (`ACTYM_COD` ASC, `MET_COD` ASC) ,
  CONSTRAINT `fk_3dfc790c-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `NMONTH` )
    REFERENCES `impact`.`plotmantim` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfd4490-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `impact`.`mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfe0a56-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `impact`.`actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB
PACK_KEYS = 0
ROW_FORMAT = DEFAULT;


-- -----------------------------------------------------
-- Table `impact`.`indigenousunits`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`indigenousunits` (
  `unit_cod` CHAR(3) NOT NULL ,
  `unit_des` VARCHAR(60) NULL ,
  `kgs` DECIMAL(12,3) NULL ,
  `lts` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`unit_cod`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpprodsprod`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpprodsprod` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `FS` DECIMAL(12,3) NULL DEFAULT 0 ,
  `LF` DECIMAL(12,3) NULL DEFAULT 0 ,
  `HC` DECIMAL(12,3) NULL DEFAULT 0 ,
  `LE` DECIMAL(12,3) NULL DEFAULT 0 ,
  `OT` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `PROD_COD`) ,
  INDEX `fk_grpcrpprodsprod_cropprods1` (`CROP_COD` ASC, `PROD_COD` ASC) ,
  CONSTRAINT `fk_grpcrpprodsprod_system1`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpprodsprod_cropprods1`
    FOREIGN KEY (`CROP_COD` , `PROD_COD` )
    REFERENCES `impact`.`cropprods` (`CROP_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpforaprod`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpforaprod` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `FS` DECIMAL(12,3) NULL ,
  `LF` DECIMAL(12,3) NULL ,
  `HC` DECIMAL(12,3) NULL ,
  `LE` DECIMAL(12,3) NULL ,
  `OT` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `FORA_COD`) ,
  INDEX `fk_grpcrpforaprod_cropfora1` (`CROP_COD` ASC, `FORA_COD` ASC) ,
  CONSTRAINT `fk_grpcrpforaprod_system1`
    FOREIGN KEY (`SYS_COD` )
    REFERENCES `impact`.`system` (`SYS_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpforaprod_cropfora1`
    FOREIGN KEY (`CROP_COD` , `FORA_COD` )
    REFERENCES `impact`.`cropfora` (`CROP_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpprodssold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpprodssold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `PROD_COD`, `MARK_COD`) ,
  INDEX `fk_grpcrpprodssold_markoutlet1` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_grpcrpprodssold_grpcrpprodsprod1`
    FOREIGN KEY (`SYS_COD` , `CROP_COD` , `PROD_COD` )
    REFERENCES `impact`.`grpcrpprodsprod` (`SYS_COD` , `CROP_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpprodssold_markoutlet1`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpprodssoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpprodssoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `qty` DECIMAL(12,3) NULL ,
  `pri` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `PROD_COD`, `MARK_COD`, `period_id`) ,
  INDEX `fk_grpcrpprodssoldperiods_periods1` (`period_id` ASC) ,
  CONSTRAINT `fk_grpcrpprodssoldperiods_grpcrpprodssold1`
    FOREIGN KEY (`SYS_COD` , `CROP_COD` , `PROD_COD` , `MARK_COD` )
    REFERENCES `impact`.`grpcrpprodssold` (`SYS_COD` , `CROP_COD` , `PROD_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpprodssoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpforasold`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpforasold` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `FORA_COD`, `MARK_COD`) ,
  INDEX `fk_grpcrpforasold_markoutlet1` (`MARK_COD` ASC) ,
  CONSTRAINT `fk_grpcrpforasold_grpcrpforaprod1`
    FOREIGN KEY (`SYS_COD` , `CROP_COD` , `FORA_COD` )
    REFERENCES `impact`.`grpcrpforaprod` (`SYS_COD` , `CROP_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpforasold_markoutlet1`
    FOREIGN KEY (`MARK_COD` )
    REFERENCES `impact`.`markoutlet` (`MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpforasoldperiods`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpforasoldperiods` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `MARK_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `qty` DECIMAL(12,3) NULL ,
  `pri` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `FORA_COD`, `MARK_COD`, `period_id`) ,
  INDEX `fk_grpcrpforasoldperiods_periods1` (`period_id` ASC) ,
  CONSTRAINT `fk_grpcrpforasoldperiods_grpcrpforasold1`
    FOREIGN KEY (`SYS_COD` , `CROP_COD` , `FORA_COD` , `MARK_COD` )
    REFERENCES `impact`.`grpcrpforasold` (`SYS_COD` , `CROP_COD` , `FORA_COD` , `MARK_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpforasoldperiods_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpprodscons`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpprodscons` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `PROD_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `qty` DECIMAL(12,3) NULL ,
  `pri` DECIMAL(12,3) NULL ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `PROD_COD`, `period_id`) ,
  INDEX `fk_grpcrpprodscons_periods1` (`period_id` ASC) ,
  CONSTRAINT `fk_grpcrpprodscons_grpcrpprodsprod1`
    FOREIGN KEY (`SYS_COD` , `CROP_COD` , `PROD_COD` )
    REFERENCES `impact`.`grpcrpprodsprod` (`SYS_COD` , `CROP_COD` , `PROD_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpprodscons_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`grpcrpforacons`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`grpcrpforacons` (
  `SYS_COD` VARCHAR(27) NOT NULL ,
  `CROP_COD` CHAR(3) NOT NULL ,
  `FORA_COD` CHAR(3) NOT NULL ,
  `period_id` CHAR(2) NOT NULL ,
  `qty` DECIMAL(12,3) NULL DEFAULT 0 ,
  `pri` DECIMAL(12,3) NULL DEFAULT 0 ,
  PRIMARY KEY (`SYS_COD`, `CROP_COD`, `FORA_COD`, `period_id`) ,
  INDEX `fk_grpcrpforacons_periods1` (`period_id` ASC) ,
  CONSTRAINT `fk_grpcrpforacons_grpcrpforaprod1`
    FOREIGN KEY (`SYS_COD` , `CROP_COD` , `FORA_COD` )
    REFERENCES `impact`.`grpcrpforaprod` (`SYS_COD` , `CROP_COD` , `FORA_COD` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_grpcrpforacons_periods1`
    FOREIGN KEY (`period_id` )
    REFERENCES `impact`.`periods` (`period_id` )
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `impact`.`agegrps`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `impact`.`agegrps` (
  `vecpos` INT NOT NULL ,
  `grptyp` INT NULL ,
  `typcod` VARCHAR(45) NULL ,
  `vfrom` DECIMAL(7,3) NULL ,
  `vto` DECIMAL(7,3) NULL ,
  `energy` DECIMAL(7,3) NULL ,
  `protein` DECIMAL(7,3) NULL ,
  PRIMARY KEY (`vecpos`) )
ENGINE = InnoDB;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
