# İşletim Sitemleri Dersi Proje Ödevi

## Proje: Linux üzerinde çalıştırmak üzere basit bir kabuk programı oluşturulması

## Grup Üyeleri
- Gamze Ceylan B191210008
- Zeynep Göksoy B201210386
- Meryem Gül Toksöz B191210026
- Temur Tsulukidze B181210563
- Şerife Gönüllü B191210089

## Fonksiyonların Özellikleri

### parseSpace(char* str, char** parsed) fonksiyonu
 Gelen stringi boşluk karakterine göre parçalara ayıran fonksiyondur.

 ### processString(char* str, char** parsed)
 Parçalanış stringler builtin komutsa işler ve 0 döndürür. Builtin komut değilse 1 döndürür.

 ### built_in_commands(char** parsed) fonksiyonu
 Gelen komutların exit,showpid veya cd olma durumuna göre ilgili fonksiyonu çağırır.

 ### showpid(char** parsed) fonksiyonu 
Arkaplanda çalışan proseslerin PID'lerini gösterir.

### execution(char** parsed) fonksiyonu
Fork işlemi ile yavru proses oluşturulur. Eğer yavru proses ise execvp ile gelen komut yavru prosesin üstüne yazılır. Eğer ebeveyn proses ise waitpid ile yavru prosesin sonlanmasını bekler.

### init_shell() fonksiyonu
Kabuğu başlatır ve prompt'urenkli olarak ekrana yazar ve eğer processString fonksiyonuyla gelen değer 1 ise komutu execution fonksiyonuna gönderir. Her çalışmada terminali temizler. Gelen komutun 80 karakterden fazla olup olmadığını kontrol eder.

### main() fonksiyonu 
init_shell() fonksiyonunu çağırır.

### Program nasıl çalışır?
Komut isteminde ödev klasörümüzün içinden "make" komutunu çalıştırdığımız zaman ödev istediğimiz şekilde çalışır.

### Kullanılan Kaynaklar
https://brennan.io/2015/01/16/write-a-shell-in-c/



 
