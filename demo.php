    <?php

    // 使用‘0’补全/截取字符串到指定长度
    function matchPading($src, $size) {
        $des = $src;

        while (strlen($des) < $size) {
            $des = $des.'0';
        }

        if (strlen($des) > $size) {
            $des = substr($des, 0, $size);
        }

        return $des;
    }

    // 使用‘0’补全字符串到指定长度倍数
    function multiplePading($src, $size) {
        $des = $src;

        $len = strlen($des);

        $deslen = $len - ($len % $size) + $size;

        while (strlen($des) < $deslen) {
            $des = $des.'0';
        }

        return $des;
    }

    function encrypt($pstr, $pkey, $piv) {

        $type = MCRYPT_RIJNDAEL_128;
        $mode = MCRYPT_MODE_CBC;

        $str = $pstr;
        $key = $pkey;
        $iv = $piv;

        $size = mcrypt_get_block_size($type, $mode);

        $str = multiplePading($str, $size);
        $key = matchPading($key, $size);
        $iv = matchPading($iv, $size);

        echo "str:".$str;
        echo "<br>";
        echo "key:".$key;
        echo "<br>";
        echo "iv:".$iv;
        echo "<br>";

        $data = mcrypt_encrypt($type, $key, $str, $mode, $iv);
        return base64_encode($data);
    }

    $key = "asdfghjkasdfghjkasdfghjkasdfghjkasdfghjkasdfghjkasdfghjkasdfghjk";
    $iv = $key;
    $str = "asdfghjkasdfghjkasdfghjkasdfghjkasdfghjkasdfghjk";

    echo encrypt($str, $key, $iv);

    ?>
