<?php
    $err = "";
    if(isset($_POST['submit'])){
	$value = $_POST['band-value'];
        if(empty($value)){
	    $err = "Escolha uma nota!";
	}else{
            $ip = $_SERVER['REMOTE_ADDR'];
            $mac = `arp -a $ip | cut -d ' ' -f 4 | tr -d '\n'`;
	    $date = date('m/d/Y h:i:s a', time());
            $db = new PDO ( 'sqlite:/home/pi/bandas.db' );
            $db->exec("INSERT INTO eletrica_logs(horario, mac, voto) VALUES('$date', '$mac', '$value')");	    
            $err = "Seu voto foi enviado com sucesso!";
        }
    }
?>

<!DOCTYPE html>
    <html lang="pt-br">
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <link rel="stylesheet" type="text/css" href="styles.css">

    <body>

    <img src="banner_desafio_min.jpg" alt="Desafio Cultural 2019">	

	<h2>Qual sua nota para a banda?</h2>
        <form action="#" method="post"><p>
        <fieldset>
        <input class="radio-input" type="radio" name="band-value" value="1"/>
	<label class="radio-label">1</label><br>
        <input class="radio-input" type="radio" name="band-value" value="2"/>
        <label class="radio-label">2</label><br>
        <input class="radio-input" type="radio" name="band-value" value="3"/>
        <label class="radio-label">3</label><br>
        <input class="radio-input" type="radio" name="band-value" value="4"/>
        <label class="radio-label">4</label><br>
        <input class="radio-input" type="radio" name="band-value" value="5"/>
        <label class="radio-label">5</label><br>    
        </fieldset>
        </p>
        <p>
        <input class="button-input" type="submit" name="submit" value="Votar!" />
                </p>
                <p class="warning"><?php echo !empty($err)?$err:"&nbsp;";?></p>
            </form>
	<script type="text/javascript" src="radios.js"></script>
    </body>
</html>
