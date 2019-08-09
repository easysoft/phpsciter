<?php

class mysql_pdo
{
    private static $self = null;

    private $dbh;

    /**
     * @return mysql_pdo
     */
    static public function instance()
    {
        if (self::$self == null) {
            self::$self = new self();
        }

        return self::$self;
    }

    public function __construct()
    {
        $this->dbh = new PDO('mysql:host=10.0.3.161;port=3306;dbname=db_license', 'root', '123456', array(PDO::ATTR_PERSISTENT => false));
    }

    public function testQuery($limit)
    {
        $rs     = $this->dbh->query('select user_id,user_company from license_user_info limit ' . (int)$limit);
        $result = $rs->fetchAll();

        return $result;
    }

    public function testPrepare()
    {
        $rs   = $this->dbh->prepare('select user_id,user_company from license_user_info where 1=1');
        $exec = $rs->execute();
        if ($exec) {
            $result = $rs->fetchAll(PDO::FETCH_ASSOC);
        }

        return $result;
    }
}
