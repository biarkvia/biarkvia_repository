## Obecné chování
MongoDB, jakožto dokumentově orientovaná NoSQL databáze, se odlišuje od ostatních typů NoSQL databází a od tradičních relačních databází. Klíčovým rozdílem je struktura úložiště dat – MongoDB ukládá data v flexibilních dokumentech ve formátu podobném JSON, což umožňuje složitější datové struktury a hierarchie. Na rozdíl od relačních databází, které vyžadují pevně definované schéma, MongoDB dovoluje dynamické schéma, čímž zjednodušuje práci s různorodými a proměnlivými daty.

## Základní principy
MongoDB podporuje distribuované datové struktury, což umožňuje horizontální škálování pomocí shardingu. Sharding rozděluje data do menších částí a distribuuje je mezi více serverů, což zlepšuje výkon a kapacitu systému. Dále MongoDB používá replikaci pro zajištění vysoké dostupnosti a odolnosti vůči selhání. Architektura MongoDB se liší od ostatních NoSQL databází tím, že kombinuje flexibilitu dokumentově orientovaného úložiště s robustními možnostmi škálování a replikace.


## CAP teorém
Podle CAP teorému (Consistency, Availability, Partition tolerance) MongoDB umožňuje konfiguraci, která preferuje dostupnost a odolnost vůči dělení sítě (AP), i když je možné nastavit ji také pro konzistenci a odolnost vůči dělení (CP). Vzhledem k potřebám vašeho řešení, které vyžaduje vysokou dostupnost a schopnost pracovat v distribuovaném prostředí, je tento přístup ideální.

### Architektura
### Jak vypadá architektura Vašeho řešení a proč?
Routery (mongos): Moje řešení používá routery jako distribuční vrstvu, která přesměrovává dotazy na správné shardy. Toto umožňuje efektivní zpracování dotazů a poskytuje jednotný pohled na cluster.

Konfigurační servery: Tyto servery jsou klíčové pro uchovávání metadata o clusteru a rozložení dat. Jejich použití zajišťuje, že data jsou správně rozdělena a dostupná.

Shardové servery: Tyto servery skladují data, která jsou rozdělena mezi ně pro lepší škálovatelnost a výkon.

### Jak se případně liší od doporučeného používání a proč?
Moje architektura se může lišit od standardního doporučeného nastavení tím, že používáte více replikačních a shardových nodů, což je zaměřeno na zajištění vysoké dostupnosti a výkonu pro správu velkého množství dat.

### Jaký cluster pro Vaše řešení jste vytvořili a proč?
Jsem vytvořil cluster s několika shardy, které umožňují distribuovat zátěž a zvýšit výkon při práci s velkými datovými soubory. Rozhodnutí pro tento typ clusteru bylo založeno na potřebě škálovatelnosti a efektivity.

### Kolik nodů pro Vaše řešení používáte a proč?
Počet nodů byl vybrán tak, aby odpovídal potřebám vysoké dostupnosti a odolnosti vůči selhání. Používáte replikaci v každém shardu, což zvyšuje redundanci a zabezpečení dat.

### Jak využíváte pro Vaše řešení replikaci a proč?
Replikace je používána pro zajištění vysoké dostupnosti a odolnosti vůči selhání. Každý shard má replikované nody, které zajišťují, že v případě výpadku jednoho nodu budou data stále dostupná.

### Využíváte sharding a jak jej pro Vaše řešení využíváte a proč? Případně pokud jej nevyužíváte, tak proč ne?
Sharding je klíčovou součástí vašeho řešení, protože umožňuje efektivní rozdělení dat a zlepšuje výkon při práci s velkými objemy dat. Vaše rozhodnutí použít sharding je založeno na potřebě škálovatelnosti a efektivního zpracování dotazů.

### S jakými typy dat Vaše databáze pracuje, jakého jsou formátu a jak s nimi databáze nakládá?
Moje databáze pracuje s dokumenty ve formátu podobném JSON, což je vhodné pro dynamická a strukturovaná data. Tento formát umožňuje flexibilitu a složité datové struktury, což je důvodem, proč jste se rozhodli nevyužívat jiné datové struktury.

### S kolika daty Vaše databáze bude pracovat? Proč jste nezvolili další možné datové struktury pro Vaši databázi?
Moje databáza bude pracovat s velkými objemy dat, která mohou pocházet z různých zdrojů, včetně aplikací, webových služeb a externích systémů. Schopnost MongoDB efektivně zpracovávat a spravovat tato data je klíčovým důvodem pro její výběr.

### Odkud jsou data generovaná?
https://www.mockaroo.com/

### Perzistence
Moje použití MongoDB řeší perzistenci dat takto:

Primární a sekundární paměť: MongoDB ukládá data na disk, což zajišťuje jejich perzistenci. V paměti udržuje často používaná data pro rychlejší přístup a dotazování.

Načítání a ukládání dat: Data jsou načítána z disku do paměti při potřebě a ukládána zpět na disk, aby byla trvale zachována. Tento proces je optimalizován pro maximální výkon a efektivitu.

### Zabezpečení
Možnosti zabezpečení: MongoDB nabízí rozsáhlé zabezpečení, včetně šifrování dat, autentizace, autorizace, a síťového zabezpečení.

Implementace zabezpečení: Zabezpečení moje databáze zahrnuje autentizaci uživatelů, šifrování dat jak v klidu, tak při přenosu, a konfiguraci síťových pravidel pro omezení přístupu k databázovým serverům.

### Výhody a nevýhody
Výhody

- Vysoká škálovatelnost a flexibilita
- Efektivní správa a zpracování velkých objemů dat
- Rozsáhlé dotazovací možnosti

Nevýhody

- Vyšší nároky na paměť a zpracovatelský výkon pro indexaci
- Méně vhodné pro aplikace vyžadující striktní transakční integrit

### Případy užití
Účel zvolení: MongoDB byla zvolena pro její schopnost efektivně zpracovávat a spravovat velké objemy strukturovaných a semi-strukturovaných dat.

Proč ne jiná NoSQL databáze: MongoDB byla preferována před jinými NoSQL databázemi kvůli její flexibilitě, škálovatelnosti a bohatým dotazovacím možnostem.

Vhodnost použití: MongoDB je ideální pro aplikace, které potřebují rychle zpracovávat velké množství dat s různorodými strukturami. Méně vhodná je pro aplikace, které vyžadují pevně strukturovaná data a striktní transakční integritu.

## Popis vlastního datasetu
dataset obsahuje tři JSON soubory, které se týkají písní, hudebních vydavatelství a skladatelů písní. Zde je detailní popis každého souboru:

## labels.json: Tento soubor obsahuje informace o hudebních vydavatelstvích. Každý záznam zahrnuje:

id_label: Unikátní identifikátor vydavatelství. [number]
id_song: Identifikátor písně, která je s tímto vydavatelstvím spojena. [number]
name_label: Název vydavatelství. [string]
country_label: Země, ve které je vydavatelství založeno. [string]

## songs.json: Tento soubor obsahuje informace o jednotlivých písních. Každý záznam zahrnuje:

id_song: Unikátní identifikátor písně. [number]
id_label: Identifikátor vydavatelství spojeného s písní. [number]
id_songwriter: Identifikátor skladatele písně. [number]
name_song: Název písně. [string]
release_date_song: Datum vydání písně ve specifickém formátu (např. "2001-02-07T03:21:03.000Z"). [date]

## songwriters.json: Tento soubor obsahuje informace o skladatelích písní. Každý záznam zahrnuje:

id_songwriter = Unikátní identifikátor skladatele. [number]
id_song: Identifikátor písně, kterou tento skladatel napsal. [number]
username_songwriter: Uživatelské jméno skladatele. [string]
full_name_songwriter: Celé jméno skladatele. [string]
age_songwriter: Věk skladatele. [number]

## Zdroje
Pro detailní informace a technické specifikace byly použity oficiální dokumentace MongoDB, technické články a průvodce nejlepšími postupy pro MongoDB. Pro zabezpečení a škálování byly konzultovány doporučené strategie od MongoDB a příslušných bezpečnostních standardů.
https://www.kaggle.com/
https://phoenixnap.com/kb/mongodb-sharding
https://github.com/minhhungit/mongodb-cluster-docker-compose

## Závěr
Ve své práci jsem úspěšně implementoval a nakonfiguroval MongoDB databázi, která efektivně řeší potřeby správy a zpracování velkých objemů dat v dynamickém a distribuovaném prostředí. Moje architektura, využívající sharding a replikaci, zajišťuje vysokou dostupnost a odolnost vůči selhání, což je klíčové pro aplikace vyžadující spolehlivý a rychlý přístup k datům. Díky flexibilním možnostem konfigurace a robustním bezpečnostním funkcím je moje řešení připraveno čelit různým výzvám souvisejícím se správou a zabezpečením dat. 
