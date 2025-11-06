//
// Created by Olcay YILDIZ on 5.11.2025.
//
#include <FsmMorphologicalAnalyzer.h>
#include <Sentence.h>
#include <string.h>
#include <Memory/Memory.h>

#include "../src/SimpleSpellChecker.h"

int main() {
    start_x_large_memory_check();
    char *originals[15] = {
        "yeni sezon başladı",
        "sırtıkara adındaki canlı ; bir balıktır", "siyah ayı ; ayıgiller familyasına ait bir ayı türüdür",
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
        "bu son model cihaz 24 inç ekran büyüklüğünde ve 9 kg ağırlıktadır"
    };
    Sentence_ptr *modified = (Sentence_ptr *) malloc_(16 * sizeof(Sentence_ptr));
    modified[0] = create_sentence3("yenisezon başladı");
    modified[1] = create_sentence3("sırtı kara adındaki canlı ; bir balıktır");
    modified[2] = create_sentence3("siyahayı ; ayıgiller familyasına ait bir ayı türüdür");
    modified[3] = create_sentence3("yeni se zon başladı gibs");
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
    Fsm_morphological_analyzer_ptr fsm = create_fsm_morphological_analyzer3();
    Simple_spell_checker_ptr spell_checker = create_simple_spell_checker(fsm);
    for (int i = 0; i < 15; i++) {
        Sentence_ptr new_sentence = spell_check_simple(spell_checker, modified[i]);
        char* st = sentence_to_string(new_sentence);
        if (strcmp(originals[i], st) != 0) {
            printf("%s\n", st);
        }
        free_(st);
        free_sentence(new_sentence);
    }
    for (int i = 0; i < 15; i++) {
        free_sentence(modified[i]);
    }
    free_(modified);
    free_fsm_morphological_analyzer(fsm);
    free_simple_spell_checker(spell_checker);
    end_memory_check();
}
