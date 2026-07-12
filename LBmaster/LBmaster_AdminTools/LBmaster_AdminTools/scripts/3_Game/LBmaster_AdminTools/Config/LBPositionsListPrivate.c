class LBPositionsListPrivate : LBConfigLoader<LBPositionsListPrivate_> {

	override void InitVars() {
		InitVarsInternal("AdminTool", "teleportpositions.json", LBConfigType.CONFIG, false);
	}

	override string GetRequiredVersion() {
		return "Admin_Tools";
	}

	override bool IsClientSideConfig() {
		return true;
	}

}
class LBPositionsListPrivate_ : LBPositionsListBase {

	static const int CURRENT_VERSION = 1;
	ref map<string, ref array<ref LB_TeleportPos>> positionsPerMap = new map<string, ref array<ref LB_TeleportPos >> (); // List of all positions per map

	[NonSerialized()]
	ref array<ref LB_TeleportPos> positions = new array<ref LB_TeleportPos>();

	override bool OnLoad() {
		SetToCurrentMap();
		return false;
	}

	void SetToCurrentMap() {
		string mapName;
		g_Game.GetWorldName(mapName);
		SetPositions(mapName);
	}

	void SetPositions(string mapName) {
		mapName.ToLower();
		if (positionsPerMap.Find(mapName, positions))
			return;
		positions = new array<ref LB_TeleportPos>();
		positionsPerMap.Insert(mapName, positions);
	}

	override void LoadDefault() {
		SetPositions("chernarusplus");
		positions.Insert(LB_TeleportPos.Init("Altar", "8160.34619140625 474.0400085449219 9099.30859375", "-7.996692180633545 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Balota Airfield", "5114.3212890625 10.2877779006958 2431.4619140625", "-92.9707260131836 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Balota", "4477.0537109375 6.046121120452881 2454.1083984375", "19.38682746887207 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Bashnya", "4088.917724609375 386.053466796875 11813.6455078125", "-122.4804458618164 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Belaya Polyana", "14032.169921875 139.58953857421876 15048.2958984375", "21.85386085510254 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Berezino", "12945.0869140625 6.15805721282959 10141.3564453125", "-73.10304260253906 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Bogatyrka", "1512.2303466796876 301.2035827636719 8954.642578125", "100.78373718261719 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Bor", "3321.35400390625 186.21788024902345 3912.1494140625", "-83.33224487304688 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Chernaya Polyana", "12143.837890625 94.27391815185547 13818.0556640625", "114.33143615722656 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Chernogorsk", "6382.5615234375 9.286691665649414 2693.904541015625", "165.54689025878907 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Devils Castle", "6899.88818359375 405.976806640625 11423.923828125", "-67.361572265625 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Dolnina", "11202.2001953125 76.65416717529297 6593.03173828125", "-4.324498176574707 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Dubrovka", "10353.45703125 129.0831298828125 9808.5419921875", "-35.50288391113281 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Elektrozavodsk", "10420.83984375 6.155354022979736 2247.3310546875", "-65.13423156738281 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Gorka", "9417.8095703125 303.7859191894531 8813.7822265625", "7.196944236755371 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Grishino", "5995.08642578125 267.07977294921877 10375.8408203125", "138.25637817382813 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Guglovo", "8502.7255859375 356.9777526855469 6689.86669921875", "-151.82662963867188 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Gvozdno", "8596.828125 198.10963439941407 11881.986328125", "48.545719146728519 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Kabanino", "5323.73828125 333.494384765625 8585.0185546875", "13.223955154418946 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Kamenka", "1830.045654296875 6.1952691078186039 2227.71728515625", "131.0834197998047 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Kamensk", "6601.82666015625 376.32861328125 14487.0966796875", "-76.89422607421875 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Kamyshovo", "12024.3564453125 6.3846259117126469 3475.650146484375", "-32.42924499511719 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Khelm", "12310.0625 104.8328857421875 10881.642578125", "-48.1917724609375 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Klen", "11474.5048828125 342.2921142578125 11336.341796875", "59.20552444458008 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Kozlovka", "4368.6650390625 234.22193908691407 4682.69677734375", "-168.5741729736328 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Krasnostav Airfield", "12159.658203125 140.01199340820313 12650.9306640625", "-133.9420166015625 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Krasnostav", "11211.1591796875 195.49087524414063 12198.775390625", "131.04490661621095 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Lopatino", "2707.133544921875 274.07470703125 10004.9521484375", "-43.23977279663086 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Mogilevka", "7551.6123046875 214.14608764648438 5158.34130859375", "-137.77841186523438 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Msta", "11305.9912109375 272.5916442871094 5592.06884765625", "169.97373962402345 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Myshkino Tents", "1159.45556640625 185.37823486328126 7272.208984375", "-170.43907165527345 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Myshkino", "1984.91162109375 254.1344451904297 7335.86865234375", "86.60157012939453 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nadezhda", "7105.7646484375 264.4937438964844 6938.16015625", "79.44143676757813 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nadezhdino", "5881.8017578125 134.26475524902345 4828.560546875", "23.695154190063478 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nagornoye", "9326.205078125 234.09498596191407 14564.4609375", "-168.02130126953126 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nizhneye", "12990.8291015625 6.031898021697998 8301.1181640625", "-134.20396423339845 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("North west Airfield", "4462.197265625 339.03143310546877 10237.69921875", "42.306156158447269 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Novaya Petrovka", "3421.9345703125 198.19000244140626 12842.0126953125", "48.0914306640625 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Novodmitrovsk", "11446.1689453125 47.60637664794922 14485.80078125", "171.31541442871095 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Novy Sobor", "7137.166015625 294.1379699707031 7691.66552734375", "-30.7166805267334 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Pavlovo Military Base", "2068.81689453125 110.84893798828125 3392.10791015625", "109.12651824951172 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Pavlovo", "1697.5078125 144.08538818359376 3795.790771484375", "47.54230499267578 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Pogorevka", "4495.47802734375 293.7665710449219 6423.77197265625", "-171.546630859375 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Polyana", "10658.3173828125 210.94967651367188 8014.61962890625", "118.59345245361328 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Pusta", "9193.7724609375 221.77383422851563 3814.856201171875", "-66.5040283203125 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Pustoshka", "3090.213623046875 303.7773132324219 7980.998046875", "-32.4923210144043 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Rify", "13602.291015625 8.503093719482422 11277.46875", "120.94123840332031 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Rogovo", "4787.46240234375 269.7905578613281 6779.453125", "-23.602073669433595 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Romashka", "8117.68408203125 252.4870147705078 10963.8076171875", "-104.2740707397461 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Severograd", "8197.912109375 112.27872467041016 12754.6533203125", "-7.628007888793945 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Sinystok", "1496.4451904296876 281.11236572265627 11922.7861328125", "98.3891372680664 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Skalisty", "13602.498046875 40.49406814575195 3061.28466796875", "-25.8726749420166 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Solnechny", "13471.7109375 5.978182792663574 6252.6591796875", "-55.45032501220703 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Sosnovka", "2520.790283203125 307.8352355957031 6393.4951171875", "154.2030487060547 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Staroye", "10092.7216796875 245.43934631347657 5487.87158203125", "37.56557083129883 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Stary Sobor", "6045.095703125 300.9249572753906 7807.369140625", "-128.0749969482422 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Stary Yar", "5048.44970703125 477.05694580078127 15043.017578125", "-152.21873474121095 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Svergino", "9667.626953125 76.59953308105469 13777.373046875", "-70.80694580078125 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Svetloyarsk", "13846.71484375 20.685571670532228 13261.095703125", "-100.77334594726563 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Tisy Military", "1671.5380859375 451.741943359375 14120.3671875", "16.17979621887207 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Tisy", "3426.35400390625 286.67999267578127 14774.5078125", "-1.2651159763336182 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Topolniki", "2913.534423828125 213.77374267578126 12454.5693359375", "96.72943115234375 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Vavilovo", "2201.54736328125 265.5202941894531 11039.8544921875", "53.098995208740237 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Vybor", "3849.548095703125 311.04180908203127 8855.9150390625", "-166.34292602539063 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Vyshnoye", "6578.98486328125 321.73583984375 6010.990234375", "32.111690521240237 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Zabolotye", "1149.08544921875 344.69287109375 10022.4931640625", "165.57534790039063 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Zaprudnoye", "5387.44677734375 160.1536865234375 12628.083984375", "175.11119079589845 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Zelenogorsk", "2519.354736328125 190.38169860839845 5276.15966796875", "158.8855743408203 0.0 -0.0"));
		SetPositions("enoch");
		positions.Insert(LB_TeleportPos.Init("Airfield", "4018.846923828125 238.17190551757813 10235.94140625", "111.36882781982422 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Bielawa", "1475.052490234375 273.38446044921877 9765.7900390625", "-175.80123901367188 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Borek", "9815.5703125 209.5564727783203 8540.6337890625", "24.12812042236328 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Branzow Castle", "1040.2698974609376 439.80718994140627 11425.0087890625", "-163.11363220214845 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Brena", "6460.27294921875 184.51048278808595 11240.9541015625", "78.140625 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Dambog", "564.3921508789063 612.6497192382813 1130.1085205078126", "92.7923583984375 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Dolnik", "11387.4677734375 215.85545349121095 611.8430786132813", "-92.53658294677735 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Gieraltow", "11519.5771484375 330.00018310546877 4615.671875", "120.57293701171875 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Gliniska", "5035.39794921875 192.68234252929688 9958.802734375", "-42.24204635620117 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Grabin", "10620.2470703125 180.33909606933595 11070.8876953125", "23.30338478088379 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Huta", "5165.2509765625 360.72723388671877 5498.08935546875", "-108.11724853515625 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Kamieniolom Maksymilian", "6467.17138671875 307.524658203125 4849.89697265625", "0.0 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Karlin", "10074.0205078125 288.8644714355469 6925.2744140625", "-117.8873519897461 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Kolembrody", "8411.5634765625 178.2115936279297 12029.2802734375", "128.30674743652345 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Konopki", "11499.36328125 346.9375 2988.898193359375", "-117.62034606933594 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Krsnik", "7775.29052734375 339.13189697265627 10071.1962890625", "4.207486152648926 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Lembork", "8844.0185546875 316.10675048828127 6666.54443359375", "121.87193298339844 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Lipina", "5930.134765625 276.84405517578127 6812.705078125", "73.5788345336914 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Lukow", "3689.11279296875 251.42039489746095 11823.4208984375", "-135.72657775878907 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Muratyn", "4545.91455078125 301.4254455566406 6480.13330078125", "-158.25201416015626 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nadbor", "5915.22900390625 397.43341064453127 4110.8974609375", "118.54761505126953 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nidek", "5994.2099609375 218.864990234375 8148.05810546875", "168.79786682128907 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Olszanka", "4772.529296875 291.9416198730469 7604.19677734375", "-104.42149353027344 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Polana", "3369.5009765625 487.874755859375 2059.5791015625", "-72.77493286132813 -0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Prison", "5557.76806640625 306.43505859375 8752.6875", "-67.97344207763672 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Pulkrabiec", "11893.5009765625 326.95220947265627 6577.61865234375", "-87.31661987304688 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Radacz", "4027.45703125 208.3164825439453 7944.341796875", "107.81349182128906 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Radunin", "7410.017578125 225.69029235839845 6442.11328125", "-138.72984313964845 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Rodzanica", "8888.67578125 475.6700439453125 2029.1253662109376", "50.517723083496097 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Roztoka", "7667.134765625 240.21421813964845 5312.560546875", "-131.9033966064453 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Sarnowek", "3177.719482421875 405.3937072753906 4939.892578125", "-130.8754119873047 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Sitnik", "11411.0419921875 183.32691955566407 9579.26171875", "40.28427505493164 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Sobotka", "6293.26171875 192.57029724121095 10223.2177734375", "59.76349639892578 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Swarog", "5028.19482421875 517.4204711914063 2138.236328125", "-91.69243621826172 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Tarnow", "9276.8779296875 199.79165649414063 10841.1328125", "-59.08536911010742 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Topolin", "1805.73291015625 200.60507202148438 7333.93505859375", "6.597342014312744 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Underground Bunker", "788.6444091796875 531.3944702148438 1226.390625", "-92.1278305053711 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Wrzeszcz", "9077.9736328125 339.005859375 4377.2529296875", "32.12413787841797 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Zalesie", "842.0791015625 242.28292846679688 5510.68798828125", "47.87287139892578 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Zapadlisko", "7995.06689453125 254.6206512451172 8735.68359375", "94.89045715332031 0.0 -0.0"));
		SetPositions("sakhal");
		positions.Insert(LB_TeleportPos.Init("Aniva", "12787.4541015625 7.777705192565918 7363.47998046875", "-57.7584114074707 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Ayah Island", "1164.849365234375 6.498962879180908 3000.11083984375", "38.56953811645508 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Bolotnoe", "5070.03857421875 3.6377251148223879 8639.46484375", "-19.004878997802736 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Cerapacha Island", "823.21337890625 31.561838150024415 11244.314453125", "-125.8836441040039 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Dolinovka", "9816.939453125 53.76484298706055 9843.2919921875", "7.782421112060547 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Dudino", "6130.2529296875 3.123332977294922 7260.11083984375", "-22.81085968017578 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Ehzarovo Island", "13442.0126953125 13.113971710205079 4995.99169921875", "156.0742645263672 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Geothermal Power Plant", "8329.318359375 30.801986694335939 10289.59375", "72.81039428710938 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Goryachevo", "8845.26171875 32.145172119140628 10025.244140625", "67.17111206054688 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Jasnomorsk", "6999.68603515625 5.485016822814941 13378.419921875", "-37.76687240600586 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Jevai", "7975.53662109375 4.471553802490234 13588.162109375", "152.0969696044922 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Juznoe", "10928.89453125 7.727987766265869 6299.953125", "93.9115982055664 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Ketoj Island", "5855.9814453125 11.472460746765137 2051.572998046875", "96.40312957763672 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Lesogorovka", "10993.7373046875 102.39913940429688 9661.5341796875", "-14.874131202697754 -0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Matrosovo", "14188.3271484375 11.949019432067871 11629.8486328125", "85.19392395019531 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Military North", "2681.289306640625 15.795486450195313 5994.04248046875", "-118.85647583007813 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Military South", "5193.9140625 3.0189809799194338 4506.21630859375", "107.55801391601563 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Mrak Island", "8698.556640625 12.635825157165528 1343.647216796875", "0.0 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Neran", "2663.033203125 5.912001132965088 9281.9658203125", "101.61149597167969 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nogovo Airfield", "7168.97412109375 2.6600000858306886 7312.8388671875", "0.0 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Nogovo", "7868.4560546875 6.1919989585876469 7794.9208984375", "-179.96835327148438 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Orlovo", "10353.0654296875 7.924036979675293 13379.2607421875", "114.96791076660156 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Petropavlovsk-Sakhalsky", "5658.09521484375 2.802000045776367 10341.9541015625", "27.493907928466798 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Pik Vostocnij", "12792.8203125 285.20196533203127 9403.5869140625", "39.10440444946289 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Podgornoe", "11004.556640625 5.437708854675293 13292.7568359375", "166.3013916015625 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Research Lab", "9667.0927734375 352.1349182128906 12146.1123046875", "128.89480590820313 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Rudnogorsk", "13291.984375 2.7669661045074465 12107.3662109375", "-122.54817962646485 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Rybnoe", "12387.21875 5.095067024230957 12688.3427734375", "11.527335166931153 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Sachalag", "12067.294921875 179.49200439453126 9752.6064453125", "125.80244445800781 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Severomorsk", "9558.6025390625 3.3135790824890138 13519.94921875", "-5.742472171783447 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Shantar Island", "2327.208740234375 14.859410285949707 3383.48779296875", "-1.3303200006484986 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Solisko Island", "12585.185546875 4.746470928192139 2408.56591796875", "-54.264068603515628 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Sovetskoe", "6396.1298828125 2.2899088859558107 12870.791015625", "-118.11713409423828 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Sumnoe", "14358.4677734375 9.076986312866211 8894.4462890625", "152.4567108154297 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Taranay", "9882.3427734375 7.287596225738525 6539.8037109375", "170.01199340820313 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Tichoe", "6219.92138671875 40.618526458740237 8642.44140625", "-101.24282836914063 -0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Tumanovo", "8436.326171875 11.532941818237305 13619.0205078125", "-19.717805862426759 -0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Tungar Island", "12643.7265625 48.60634231567383 14220.7109375", "151.33523559570313 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Urup Island", "1712.96923828125 43.44841003417969 874.3729858398438", "-13.684304237365723 0.0 0.0"));
		positions.Insert(LB_TeleportPos.Init("Uzhki Island", "10964.59765625 19.9589786529541 14544.5302734375", "-144.817626953125 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Vajkovo", "14498.2421875 10.936389923095704 9816.3681640625", "136.65768432617188 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Vostok", "13923.72265625 6.238521099090576 8387.849609375", "159.8612060546875 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Vulkano Top", "10026.740234375 556.76171875 12093.5732421875", "-172.16944885253907 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Vysokoe", "11071.966796875 113.3927001953125 8035.7216796875", "128.66903686523438 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Zupanovo", "5746.705078125 2.696141004562378 12661.1083984375", "130.63414001464845 0.0 -0.0"));
		positions.Insert(LB_TeleportPos.Init("Zvezdny nikel", "11903.63671875 147.16200256347657 10265.005859375", "27.224153518676759 0.0 -0.0"));
	}

	override array<ref LB_TeleportPos> GetPositions() {
		return positions;
	}

	override void RemovePosition(LB_TeleportPos pos) {
		positions.RemoveItem(pos);
		Save(pos);
	}

	override void AddPosition(LB_TeleportPos pos) {
		positions.Insert(pos);
		Save(pos);
	}

	override void Save(LB_TeleportPos pos) {
		LBPositionsListPrivate.Loader.Save();
	}

	override void ChangePosition(LB_TeleportPos pos, LB_TeleportPos old) {
		Save(pos);
	}

}