# Elastic Stack

## Stručný název zvoleného datasetu

songs

### Popis datasetu
[100 000 záznamů] list_of_songs.csv

Obsahuje informace o písních v 8 sloupcích:

id = Identifikátor písně [celé číslo]
song_name = Název písně [řetězec].
album_name = Název alba [řetězec].
artists_name = Jméno umělce nebo skupiny [řetězec].
danceability = Tančivost písně [float].
tempo = Tempo písně [float].
duration_ms = Délka písně v milisekundách [celé číslo].
year = Rok vydání [celé číslo].
release_date = Datum vydání [datum].

[263 záznamů] tiktok_songs.csv

Obsahuje informace o populárních písních na TikToku v 14 sloupcích:

song_name = Název písně [řetězec].
artists_name = Jméno umělce nebo skupiny [řetězec].
artist_pop = Popularita umělce [celé číslo].
album_name = Název alba [řetězec].
danceability = Tančivost písně [float].
energy = Energie písně [float].
loudness = Hlasitost písně [float].
speechiness = Míra slovnosti v písni [float].
acousticness = Akustičnost písně [float].
instrumentalness = Instrumentálnost písně [float].
liveness = Živost písně [float].
valence = Pozitivita písně [float].
tempo = Tempo písně [float].
duration_ms = Délka písně v milisekundách [celé číslo].
 
[1000 záznamů] top_1000_songs.csv

Obsahuje informace o 1000 nejlepších písních v 10 sloupcích:

uri = Jedinečný identifikátor písně [řetězec].
song_name = Název písně [řetězec].
artists_name = Jméno umělce nebo skupiny [řetězec].
album_name = Název alba [řetězec].
release_date = Datum vydání [datum].
popularity = Popularita písně [celé číslo].
genre = Hudební žánr [řetězec].
danceability = Tančivost písně [float].
label = Hudební vydavatelství [řetězec].
copyrights = Autorská práva [řetězec].

### Zdroje
Data byla získána z:
list_of_songs.csv = https://www.kaggle.com/datasets/rodolfofigueroa/spotify-12m-songs
tiktok_songs.csv = https://www.kaggle.com/datasets/joebeachcapital/top-10000-spotify-songs-1960-now
top_1000_songs.csv = https://www.kaggle.com/datasets/sveta151/tiktok-popular-songs-2022

### Formát dat
Datasety jsou ve formátu CSV, s různými sloupci pro každý dataset. Například, 'list_of_songs.csv' obsahuje sloupce jako 'id', 'song_name', 'artists_name' atd., zatímco 'tiktok_songs.csv' a 'top_1000_songs.csv' mají své specifické sloupce odpovídající jejich zaměření.

### Provedené úpravy dat
Data byla upravena a normalizována pro lepší analýzu. Byly provedeny úpravy jako konverze datových typů, normalizace textových řetězců a odstranění nepotřebných dat.

## Závěr
V rámci tohoto projektu byla provedena komplexní práce na vytvoření a integraci datasetů, dotazů Elasticsearch a skriptů pro efektivní využití v prostředí Kibana. Tato práce zahrnuje následující klíčové aspekty:
Příprava Datasets: Byly nalezeny a upraveny tři hlavní datasety ('list_of_songs.csv', 'tiktok_songs.csv', 'top_1000_songs.csv'), které poskytují detailní pohled na různé aspekty hudebních stop. Tyto datasety byly pečlivě připraveny s ohledem na potřeby analýzy a vizualizace v Kibana.
Konfigurace Logstash: Pro efektivní import dat do Elasticsearch byly vytvořeny konfigurační soubory Logstash. Tyto soubory řídí zpracování a transformaci dat před jejich uložením do Elasticsearch.
Vytváření Dotazů Elasticsearch: Pro hlubší analýzu a extrakci informací z dat byly vytvořeny specifické dotazy Elasticsearch. Tyto dotazy umožňují uživatelům provádět komplexní vyhledávání a analýzy dat přímo v Kibana.
Vývoj Skriptů: Pro automatizaci a zjednodušení procesů byly vyvinuty skripty.
Vizualizace v Kibana: Nakonec byly využity funkce Kibana pro vytvoření intuitivních a informačních vizualizací, které umožňují uživatelům snadno interpretovat analyzovaná data.
