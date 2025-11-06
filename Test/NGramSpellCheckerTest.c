//
// Created by Olcay YILDIZ on 6.11.2025.
//
#include <FsmMorphologicalAnalyzer.h>
#include <NGram.h>
#include <string.h>
#include <NoSmoothing.h>
#include <Memory/Memory.h>

#include "../src/NGramSpellChecker.h"

int main() {
    char *originals[40] = {
        "yeni sezon başladı",
        "sırtıkara adındaki canlı ; bir balıktır",
        "siyah ayı ; ayıgiller familyasına ait bir ayı türüdür",
        "yeni sezon başladı gibi",
        "alışveriş için markete gitti",
        "küçük bir yalıçapkını geçti",
        "meslek odaları birliği yeniden toplandı",
        "yeni yılın sonrasında vakalarda artış oldu",
        "atomik saatin 10 mhz sinyali kalibrasyon hizmetlerinde referans olarak kullanılmaktadır",
        "rehberimiz bu bölgedeki çıngıraklı yılan varlığı hakkında konuştu",
        "bu haksızlık da unutulup gitmişti",
        "4'lü tahıl zirvesi İstanbul'da gerçekleşti",
        "10'lu sistemden 100'lü sisteme geçiş yapılacak",
        "play-off maçlarına çıkacak takımlar belli oldu",
        "bu son model cihaz 24 inç ekran büyüklüğünde ve 9 kg ağırlıktadır",
        "yardımcı olur musunuz ?",
        "buraya daha önce gelmemiş miydik ?",
        "kutunun boyutları 0.2 m x 0.3 m x 0.5 m olacak",
        "2 tb depolama alanına sahip 7200 rpm bir disk",
        "anahtarlarımı Kadıköy'de bir lokantada unutmuşum",
        "bütün suç Selma'da değil",
        "Fransa'nın başkenti Paris'tir",
        "Nişantaşı'ndan Kadıköy'e gitmek için metroya binip Üsküdar'da inmek gerekiyor",
        "90'lı yıllarda ülkede çok büyük değişimler oldu",
        "100'lü parçaları bir araya getirerek 100'lü bir resim oluşturduk",
        "size konuyla ilgili bir e-posta gönderdim",
        "meyve-sebze reyonundan bir kilo elma aldım",
        "demokratik cumhuriyet en kıymetli varlığımızdır",
        "bu tablodaki değerler zedelenmeyecektir",
        "vakfın geleneksel yılın sporcusu anketi yeni yaşını doldurdu",
        "demokrasinin icadı bu ayrımı bulandırdı",
        "dışişleri müsteşarı Öymen'in 1997'nin ilk aylarında Bağdat'a gitmesi öngörülüyor",
        "büyüdü , palazlandı , devleti ele geçirdi",
        "her maskenin ciltte kalma süresi farklıdır",
        "yılın son ayında 10 gazeteci gözaltına alındı",
        "iki pilotun kullandığı uçakta bir hostes görev alıyor",
        "son derece kısıtlı kelimeler çerçevesinde kendilerini uzun cümlelerle ifade edebiliyorlar",
        "minibüs durağı",
        "noter belgesi",
        "bu filmi daha önce görmemiş miydik diye sordu"};
    Sentence_ptr *modified = (Sentence_ptr *) malloc_(40 * sizeof(Sentence_ptr));
    modified[0] = create_sentence3("yenisezon başladı");
    modified[1] = create_sentence3("sırtı kara adındaki canlı ; bir balıktır");
    modified[2] = create_sentence3("siyahayı ; ayıgiller familyasına ait bir ayı türüdür");
    modified[3] = create_sentence3("yeni se zon başladı gibi");
    modified[4] = create_sentence3("alis veriş için markete gitit");
    modified[5] = create_sentence3("kucuk bri yalı çapkını gecti");
    modified[6] = create_sentence3("mes lek odaları birliği yenidün toplandı");
    modified[7] = create_sentence3("yeniyılın sonrasında vakalarda artış oldu");
    modified[8] = create_sentence3(
        "atomik saatin 10mhz sinyali kalibrasyon hizmetlerinde referans olarka kullanılmaktadır");
    modified[9] = create_sentence3("rehperimiz buı bölgedeki çıngıraklıyılan varlıgı hakkınd konustu");
    modified[10] = create_sentence3("bu haksızlıkda unutulup gitmişti");
    modified[11] = create_sentence3("4 lı tahıl zirvesi İstanbul' da gerçekleşti");
    modified[12] = create_sentence3("10 lük sistemden 100 lık sisteme geçiş yapılacak");
    modified[13] = create_sentence3("play - off maçlarına çıkacak takımlar belli oldu");
    modified[14] = create_sentence3("bu son model ciha 24inç ekran büyüklüğünde ve 9kg ağırlıktadır");
    modified[15] = create_sentence3("yardımcı olurmusunuz ?");
    modified[16] = create_sentence3("buraya daha önce gelmemişmiydik ?");
    modified[17] = create_sentence3("kutunun boyutları 0.2m x 0.3m x 0.5m olacak");
    modified[18] = create_sentence3("2tb depolama alanına sahip 7200rpm bir disk");
    modified[19] = create_sentence3("anahtarlarımı Kadıköyda bir lokantada unutmuşum");
    modified[20] = create_sentence3("bütün suç Selmada değil");
    modified[21] = create_sentence3("Fransanın başkenti Paristir");
    modified[22] = create_sentence3("Nişantaşından Kadıköye gitmek için metroya binip Üsküdarda inmek gerekiyor");
    modified[23] = create_sentence3("90 lü yıllarda ülkede çok büyük değişimler oldu");
    modified[24] = create_sentence3("100 lık parçaları bir araya getirerek 100 lük bir resim oluşturduk");
    modified[25] = create_sentence3("size konuyla ilgili bir e - posta gönderdim");
    modified[26] = create_sentence3("meyve — sebze reyonundan bir kilo elma aldım");
    modified[27] = create_sentence3("demokratik cumhüriyet en kımetli varlıgımızdır");
    modified[28] = create_sentence3("bu tblodaki değerler zedelenmeyecektir");
    modified[29] = create_sentence3("vakfın geeneksel yılin spoşcusu ankşti yeni yeşını doldürdu");
    modified[30] = create_sentence3("demokrasinin icşdı bu ayrmıı bulandürdı");
    modified[31] = create_sentence3("dışişleri mütseşarı Öymen'in 1997'nin iljk aylğrında Bağdat'a gitmesi öngörülüyor");
    modified[32] = create_sentence3("büyüdü , palazandı , devltei ele geçridi");
    modified[33] = create_sentence3("her makenin cültte aklma sürdsi farlkıdır");
    modified[34] = create_sentence3("yılın son ayında 10 gazteci gözlatına alündı");
    modified[35] = create_sentence3("iki piotun kulçandığı uçkata bir hotes görçv alyıor");
    modified[36] = create_sentence3("son deece kısütlı keilmeler çeçevesinde kendülerini uzuü cümllerle ifüde edbeiliyorlar");
    modified[37] = create_sentence3("minibü durağı");
    modified[38] = create_sentence3("ntoer belgesi");
    modified[39] = create_sentence3("bu filmi daha önce görmemişmiydik diye sordu");
    Fsm_morphological_analyzer_ptr fsm = create_fsm_morphological_analyzer3();
    N_gram_ptr n_gram = create_string_n_gram3("ngram.txt");
    set_probabilities_simple(n_gram, NULL, set_probabilities_with_level_no_smoothing);
    Spell_checker_parameter_ptr parameter = create_spell_checker_parameter();
    N_gram_spell_checker_ptr spell_checker = create_n_gram_spell_checker(fsm, parameter, n_gram);
    for (int i = 0; i < 40; i++) {
        Sentence_ptr new_sentence = spell_check_n_gram(spell_checker, modified[i]);
        char* st = sentence_to_string(new_sentence);
        if (strcmp(originals[i], st) != 0) {
            printf("%s\n", st);
        }
        free_(st);
        free_sentence(new_sentence);
    }
    for (int i = 0; i < 40; i++) {
        free_sentence(modified[i]);
    }
    free_(modified);
    free_fsm_morphological_analyzer(fsm);
    free_n_gram_spell_checker(spell_checker);
}
