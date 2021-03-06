CREATE  TABLE IF NOT EXISTS `plotmanheap` (
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
    REFERENCES `plotmantim` (`SYS_COD` , `PLOT_COD` , `ACTY_COD` , `NMONTH` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfd4490-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`SYS_COD` , `HEAP_COD` )
    REFERENCES `mnrheap` (`SYS_COD` , `HEAP_COD` )
    ON DELETE CASCADE
    ON UPDATE RESTRICT,
  CONSTRAINT `fk_3dfe0a56-bce2-11e0-a5d9-58946b4c242c0`
    FOREIGN KEY (`ACTYM_COD` , `MET_COD` )
    REFERENCES `actymethods` (`ACTY_COD` , `MET_COD` )
    ON DELETE RESTRICT
    ON UPDATE RESTRICT)
ENGINE = InnoDB;