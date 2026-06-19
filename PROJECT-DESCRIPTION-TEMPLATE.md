# Overcooked

183/2020 - Andrea Andrejevic  
Projekat predstavlja scenu inspirisanu igrom Overcooked, implementiranu u OpenGL-u. Scena sadrzi kuhinjski prostor
koji se sastoji od poda, zidova sa lampama, kuhinjskih elemenata i kuvara. Implementirano je direkciono osvetljenje
(directional lighting), vise point light izvora, niz vremenski povezanih dogadjaja, a od dodatnih funkcionalnosti
implementirani su off-screen anti-aliasing i point shadows.

## Controls

`W` -> pomeranje kamere unapred  
`S` -> pomeranje kamere unazad  
`A` -> pomeranje kamere ulevo  
`D` -> pomeranje kamere udesno  
`Q` -> pomeranje kamere nadole  
`E` -> pomeranje kamere nagore  
`Scroll` -> zumiranje  
`0` -> iskljucivanje lampi  
`1` -> nizak intenzitet osvetljenja  
`2` -> srednji intenzitet osvetljenja  
`3` -> visok intenzitet osvetljenja  
`C` -> pokretanje niza vremenski povezanih dogadjaja  
`V` -> vracanje scene u pocetno stanje  
`Esc` -> zatvaranje aplikacije

## Features

### Fundamental:

[x] Model sa osvetljenjem  
[x] Dva tipa osvetljenja sa podesavanjem inteziteta preko tastature  
[x] `C` --- NAKON_2_SEKUNDE --- Pokrece ---> pojacavanje intenziteta svetla  
---> NAKON_2_SEKUNDE --- Pokrece ---> pomeranje kuvara do sporeta i prikaz zivog burgera  
---> NAKON_3_SEKUNDE --- Pokrece ---> zamenu zivog burgera pecenim burgerom  
---> NAKON_1.5_SEKUNDE --- Pokrece ---> pomeranje kuvara do stola i prikaz gotovog burgera

### Group A:

[ ] Frame-buffers with post-processing   
[x] Off-screen Anti-Aliasing  
[ ] Parallax Mapping
[ ] Bloom with the use of HDR

### Group B:

[ ] Deferred Shading  
[x] Point Shadows  
[ ] SSAO

### Engine improvement:

[x] Dodata `MultisampleFramebuffer` komponenta u `engine::graphics` modul za off-screen anti-aliasing  
[x] Dodata `PointShadowFramebuffer` komponenta u `engine::graphics` modul za generisanje depth cubemap-a kod point
shadows efekta  
[x] Dodate pomocne metode u klasu `OpenGL` za inicijalizaciju i iscrtavanje teksturisanog quad-a

## Models:

- Kuhinjski elementi i tekstura
  poda: https://sketchfab.com/3d-models/overcooked-kitchen-assets-fan-art-ec99c64c346347a89454f569054ddb86
- Kuvar: https://sketchfab.com/3d-models/overcooked-pug-character-564000f4aa2840e68fdeb339b8e7abce
- Lampe: https://free3d.com/3d-model/sconce-light-95541.html
- Burger: https://sketchfab.com/3d-models/raw-burger-meatball-patty-f06c05b2ed79480097a1ee0911f32344
- Napomena: za prikaz pecenog burgera koristen je isti model samo sa promenjenom bojom teksture.

## Textures

- Zidovi: https://ambientcg.com/view?id=Tiles010


