/* dessin.c : module fourni (dessin d'un arbre de profil, au format SVG, fixe ou animé) */
#include <stdio.h>
#include <stdlib.h>
#include "dessin.h"

#define LARGEUR_MAX 1200    // taille maximale de l'image, en pixels
#define HAUTEUR_MAX 900
#define MARGE 20

// paramètres du passage des coordonnées de l'arbre aux pixels de l'image
static float echelle, xmin, xmax, ymax;
static int largeur_image, hauteur_image, saison_max;

static float px(float x)
{
    return MARGE + echelle * (x - xmin);
}

// l'axe vertical du SVG va vers le bas
static float py(float y)
{
    return hauteur_image - MARGE - echelle * y;
}

/* agrandit si besoin le rectangle [xmin ; xmax] x [0 ; ymax] pour qu'il contienne
   l'extrémité de r et celles de tous ses descendants, et note la dernière saison (fonction récursive) */
static void updateBounds(t_rameau *r)
{
    if (r->x < xmin)
    {
        xmin = r->x;
    }
    if (r->x > xmax)
    {
        xmax = r->x;
    }
    if (r->y > ymax)
    {
        ymax = r->y;
    }
    if (r->saison > saison_max)
    {
        saison_max = r->saison;
    }
    for (int i = 0; i < r->nb_fils; i++)
    {
        updateBounds(r->p_fils[i]);
    }
}

/* calcule l'échelle et la taille de l'image, puis écrit le début du fichier SVG (fond blanc et sol) */
static void startImage(FILE *f, t_rameau *pied)
{
    // 1. rectangle qui contient l'arbre (le pied part du sol, au point (x ; 0))
    xmin = xmax = pied->x;
    ymax = pied->y;
    saison_max = 0;
    updateBounds(pied);
    xmin -= 0.5f;
    xmax += 0.5f;
    ymax += 0.5f;
    // 2. échelle : 50 pixels par unité de longueur au plus, et l'image ne dépasse pas la taille maximale
    echelle = 50;
    if (echelle * (xmax - xmin) > LARGEUR_MAX - 2 * MARGE)
    {
        echelle = (LARGEUR_MAX - 2 * MARGE) / (xmax - xmin);
    }
    if (echelle * ymax > HAUTEUR_MAX - 2 * MARGE)
    {
        echelle = (HAUTEUR_MAX - 2 * MARGE) / ymax;
    }
    largeur_image = (int) (echelle * (xmax - xmin)) + 2 * MARGE;
    hauteur_image = (int) (echelle * ymax) + 2 * MARGE;
    // 3. début de l'image : le fond et le sol
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\">\n", largeur_image, hauteur_image);
    fprintf(f, "<rect width=\"100%%\" height=\"100%%\" fill=\"white\"/>\n");
    fprintf(f, "<line x1=\"0\" y1=\"%.1f\" x2=\"%d\" y2=\"%.1f\" stroke=\"rgb(150,150,150)\" stroke-width=\"1\"/>\n",
            py(0), largeur_image, py(0));
}

/* ======================= dessin fixe ======================= */

/* dessine les segments qui vont de l'extrémité de r à celles de ses fils,
   puis dessine de la même façon chacun des fils (fonction récursive) */
static void drawBranch(FILE *f, t_rameau *r, float rayon)
{
    for (int i = 0; i < r->nb_fils; i++)
    {
        t_rameau *fils = r->p_fils[i];
        fprintf(f, "  <line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n",
                px(r->x), py(r->y), px(fils->x), py(fils->y));
        drawBranch(f, fils, rayon);
    }
    if (r->etat == BOURGEON)   // un bourgeon : un petit disque vert
    {
        fprintf(f, "  <circle cx=\"%.1f\" cy=\"%.1f\" r=\"%.1f\" fill=\"rgb(58,154,58)\" stroke=\"none\"/>\n",
                px(r->x), py(r->y), rayon);
    }
}

void drawSvg(t_rameau *pied, const char *nom_fichier)
{
    FILE *f = fopen(nom_fichier, "wt");
    if (f == NULL)
    {
        perror("Impossible de creer le fichier SVG");
        return;
    }
    startImage(f, pied);
    float epaisseur = echelle > 25 ? 3.0f : 1.0f;
    float rayon = echelle > 25 ? 5.0f : 2.0f;
    fprintf(f, "<g stroke=\"rgb(122,82,48)\" stroke-width=\"%.1f\" stroke-linecap=\"round\">\n", epaisseur);
    fprintf(f, "  <line x1=\"%.1f\" y1=\"%.1f\" x2=\"%.1f\" y2=\"%.1f\"/>\n",
            px(pied->x), py(0), px(pied->x), py(pied->y));
    drawBranch(f, pied, rayon);
    fprintf(f, "</g>\n</svg>\n");
    fclose(f);
}

/* ======================= dessin animé =======================
   Pendant toute la saison s (entre les instants s x duree et (s + 1) x duree, en secondes), les rameaux
   apparus à la saison s « poussent » : leur segment s'allonge régulièrement, sans pause d'une saison
   à la suivante. Un bourgeon apparaît quand son rameau a fini de pousser, et disparaît quand son
   premier fils a fini de pousser (les bourgeons des nouveaux rameaux prennent alors le relais).
   Les animations sont écrites en CSS, dans l'élément <style> du fichier SVG :
   - classe "r" : le segment est dessiné en pointillés (un seul trait, de la longueur du segment)
     décalés de toute leur longueur, donc invisible ; l'animation « pousse » ramène le décalage à 0 ;
   - classes "b" (bourgeon) et "t" (compteur de saisons) : invisibles au départ ; « apparait » les rend
     visibles, puis « disparait » les cache ; la classe "dernier" (bourgeons de la fin, dernière saison)
     n'a que l'animation « apparait ». */

static double duree;   // durée d'une saison, en secondes

/* écrit le segment qui va de (x1 ; y1) à (x2 ; y2), qui pousse pendant la saison s */
static void writeAnimatedSegment(FILE *f, float x1, float y1, float x2, float y2, int s)
{
    fprintf(f, "  <path class=\"r\" pathLength=\"1\" style=\"animation-delay:%.2fs\" d=\"M%.1f %.1f L%.1f %.1f\"/>\n",
            s * duree, px(x1), py(y1), px(x2), py(y2));
}

/* écrit les segments vers les fils de r et les bourgeons, puis s'appelle pour chacun des fils (fonction récursive) */
static void animateBranch(FILE *f, t_rameau *r, float rayon)
{
    for (int i = 0; i < r->nb_fils; i++)
    {
        t_rameau *fils = r->p_fils[i];
        writeAnimatedSegment(f, r->x, r->y, fils->x, fils->y, fils->saison);
        animateBranch(f, fils, rayon);
    }
    // le bourgeon de r : visible de la fin de la pousse de r jusqu'à la fin de la pousse de son premier fils
    double debut = (r->saison + 1) * duree;
    if (r->nb_fils == 0)
    {
        fprintf(f, "  <circle class=\"b dernier\" style=\"animation-delay:%.2fs\" cx=\"%.1f\" cy=\"%.1f\" r=\"%.1f\"/>\n",
                debut, px(r->x), py(r->y), rayon);
    }
    else
    {
        fprintf(f, "  <circle class=\"b\" style=\"animation-delay:%.2fs, %.2fs\" cx=\"%.1f\" cy=\"%.1f\" r=\"%.1f\"/>\n",
                debut, (r->p_fils[0]->saison + 1) * duree, px(r->x), py(r->y), rayon);
    }
}

void animateSvg(t_rameau *pied, const char *nom_fichier, double duree_saison)
{
    FILE *f = fopen(nom_fichier, "wt");
    if (f == NULL)
    {
        perror("Impossible de creer le fichier SVG");
        return;
    }
    duree = duree_saison;
    startImage(f, pied);
    float epaisseur = echelle > 25 ? 3.0f : 1.0f;
    float rayon = echelle > 25 ? 5.0f : 2.0f;
    // les animations
    fprintf(f, "<style>\n");
    fprintf(f, "  .r { fill: none; stroke-dasharray: 1 1; stroke-dashoffset: 1;\n");
    fprintf(f, "       animation: pousse %.2fs linear forwards; }\n", duree);
    fprintf(f, "  .b, .t { opacity: 0; animation: apparait 0.01s forwards, disparait 0.01s forwards; }\n");
    fprintf(f, "  .dernier { animation: apparait 0.01s forwards; }\n");
    fprintf(f, "  .b { fill: rgb(58,154,58); stroke: none; }\n");
    fprintf(f, "  @keyframes pousse    { from { stroke-dashoffset: 1; } to { stroke-dashoffset: 0; } }\n");
    fprintf(f, "  @keyframes apparait  { from { opacity: 0; } to { opacity: 1; } }\n");
    fprintf(f, "  @keyframes disparait { from { opacity: 1; } to { opacity: 0; } }\n");
    fprintf(f, "</style>\n");
    // le compteur de saisons, en haut à gauche : « Saison s » est affiché pendant la saison s
    for (int s = 0; s <= saison_max; s++)
    {
        if (s < saison_max)
        {
            fprintf(f, "<text class=\"t\" style=\"animation-delay:%.2fs, %.2fs\"", s * duree, (s + 1) * duree);
        }
        else   // la dernière saison reste affichée
        {
            fprintf(f, "<text class=\"t dernier\" style=\"animation-delay:%.2fs\"", s * duree);
        }
        fprintf(f, " x=\"10\" y=\"25\" font-family=\"sans-serif\" font-size=\"18\">Saison %d</text>\n", s);
    }
    // l'arbre : le pied (saison 0), puis tous les autres rameaux
    fprintf(f, "<g stroke=\"rgb(122,82,48)\" stroke-width=\"%.1f\">\n", epaisseur);
    writeAnimatedSegment(f, pied->x, 0, pied->x, pied->y, 0);
    animateBranch(f, pied, rayon);
    fprintf(f, "</g>\n</svg>\n");
    fclose(f);
}

/* ======================= trois vues interactives (HTML) =======================
   Le fichier HTML contient la page complète : le style, les trois dessins (vides au départ), puis un script
   qui les construit dans le navigateur à partir des rameaux de l'arbre, écrits par le programme C sous la
   forme de deux constantes JavaScript :
   - PIED = {num, x, y} ;
   - LIGNES : un rameau par ligne, [numéro, parent, saison, génération, direction, longueur, x, y],
     chaque rameau après son parent.
   Seuls les rameaux apparus au plus tard à la saison SAISON_MAX_HTML sont écrits : au-delà, les arbres
   informatiques deviennent illisibles. L'état bois ou bourgeon est recalculé par le script à partir des
   fils écrits : l'arbre affiché est donc celui de la fin de la saison SAISON_MAX_HTML. */

#define SAISON_MAX_HTML 15

/* début de la page, jusqu'à l'ouverture du script */
static const char *html_debut =
    "<!DOCTYPE html>\n"
    "<html lang=\"fr\">\n"
    "<head>\n"
    "<meta charset=\"utf-8\">\n"
    "<title>Un arbre vu de trois façons</title>\n"
    "<style>\n"
    "body{margin:0;padding:16px;background:#F4F2EC}\n"
    ".al{--al-wood:#7A4E2D;--al-wood-soft:#EBDDD0;--al-bud:#2F9A3E;--al-bud-soft:#DAF0DC;--al-hl:#E08A00;--al-hl-soft:#FFE7BF;\n"
    "  --al-ink:#1E2420;--al-muted:#7C847C;--al-grid:#E6E2D8;--al-edge:#9AA09A;\n"
    "  position:relative;padding:14px 14px 10px;border-radius:12px;background:#fff;border:1px solid #DCD8CD;\n"
    "  color:var(--al-ink);font-family:\"IBM Plex Sans\",\"Segoe UI\",system-ui,sans-serif;line-height:1.4}\n"
    ".al *{box-sizing:border-box}\n"
    ".al-titre{margin:0 10px 10px;font-size:17px}\n"
    ".al-panels{display:flex;flex-wrap:wrap;gap:12px 0;align-items:flex-start}\n"
    ".al-panel{flex:0 0 auto;max-width:100%;overflow-x:auto;display:flex;flex-direction:column;padding:0 10px}\n"
    ".al-panel+.al-panel{border-left:1px solid #E8E5DD}\n"
    ".al-panel h4{margin:0 0 2px;font-size:15px;font-weight:700}\n"
    ".al-panel p{margin:0 0 6px;font-size:12.5px;color:var(--al-muted)}\n"
    ".al svg{display:block;overflow:visible}\n"
    ".al .al-axis{fill:var(--al-muted);font-size:11px}\n"
    ".al .al-grid line{stroke:var(--al-grid);stroke-width:1}\n"
    ".al .al-sol{stroke:var(--al-edge);stroke-width:2}\n"
    "/* arbre informatique : les liens sont fins et gris — ce ne sont PAS des rameaux */\n"
    ".al .al-lien{stroke:var(--al-edge);stroke-width:1.3;stroke-dasharray:4 3}\n"
    ".al .al-noeud{cursor:pointer;outline:none}\n"
    ".al .al-noeud circle{stroke-width:2.5;transition:stroke .15s,fill .15s,r .15s}\n"
    ".al .al-noeud.bois circle{fill:var(--al-wood-soft);stroke:var(--al-wood)}\n"
    ".al .al-noeud.bourgeon circle{fill:var(--al-bud-soft);stroke:var(--al-bud)}\n"
    ".al .al-noeud text{font-size:14px;font-weight:700;fill:var(--al-ink);text-anchor:middle;dominant-baseline:central;pointer-events:none}\n"
    "/* arbre de rameaux : chaque segment est un rameau */\n"
    ".al .al-rameau{cursor:pointer}\n"
    ".al .al-rameau .al-seg{stroke:var(--al-wood);stroke-width:4;stroke-linecap:round;transition:stroke .15s,stroke-width .15s,opacity .15s}\n"
    ".al .al-rameau .al-zone{stroke:transparent;stroke-width:16;stroke-linecap:round}\n"
    ".al .al-rameau .al-bout{transition:fill .15s,opacity .15s}\n"
    ".al .al-rameau.bois .al-bout{fill:var(--al-wood)}\n"
    ".al .al-rameau.bourgeon .al-bout{fill:var(--al-bud)}\n"
    ".al .al-rameau text{font-size:12.5px;font-weight:700;fill:var(--al-ink);pointer-events:none;transition:opacity .15s}\n"
    ".al .al-depart{fill:none;stroke:var(--al-hl);stroke-width:2;stroke-dasharray:3 2;opacity:0;transition:opacity .15s}\n"
    "/* état « survol » */\n"
    ".al.actif .al-rameau:not(.hl) .al-seg,.al.actif .al-rameau:not(.hl) .al-bout{opacity:.25}\n"
    ".al.actif .al-rameau:not(.hl) text{opacity:.35}\n"
    ".al .al-rameau.hl .al-seg{stroke:var(--al-hl);stroke-width:7}\n"
    ".al .al-rameau.hl .al-bout{fill:var(--al-hl)}\n"
    ".al .al-rameau.hl text{font-size:15px}\n"
    ".al.actif .al-depart{opacity:1}\n"
    ".al .al-noeud.hl circle{fill:var(--al-hl-soft);stroke:var(--al-hl);r:24}\n"
    ".al-legende{display:flex;flex-wrap:wrap;gap:4px 18px;margin:8px 10px 0;font-size:12.5px;color:var(--al-muted)}\n"
    ".al-legende span{display:inline-flex;align-items:center;gap:6px}\n"
    ".al-legende i{display:inline-block;width:12px;height:12px;border-radius:50%;border:2px solid}\n"
    ".al-legende .l-bois{background:var(--al-wood-soft);border-color:var(--al-wood)}\n"
    ".al-legende .l-bud{background:var(--al-bud-soft);border-color:var(--al-bud)}\n"
    ".al-legende .l-lien{width:22px;height:0;border-radius:0;border:0;border-top:1.5px dashed var(--al-edge)}\n"
    ".al-legende .l-seg{width:22px;height:4px;border-radius:2px;border:0;background:var(--al-wood)}\n"
    "/* fenêtre d'information */\n"
    ".al-pop{position:absolute;z-index:5;min-width:190px;padding:10px 12px;border-radius:10px;background:#1F2620;color:#E6EDE6;\n"
    "  font-size:13px;box-shadow:0 6px 20px rgba(0,0,0,.18);pointer-events:none;opacity:0;transform:translateY(4px);transition:opacity .12s,transform .12s}\n"
    ".al-pop.on{opacity:1;transform:none}\n"
    ".al-pop b{display:block;margin-bottom:6px;font-size:14px}\n"
    ".al-pop b em{font-style:normal;font-weight:600;font-size:11.5px;margin-left:8px;padding:1px 8px;border-radius:999px}\n"
    ".al-pop b em.bois{background:var(--al-wood-soft);color:var(--al-wood)}\n"
    ".al-pop b em.bourgeon{background:var(--al-bud-soft);color:var(--al-bud)}\n"
    ".al-pop dl{display:grid;grid-template-columns:auto auto;gap:2px 14px;margin:0}\n"
    ".al-pop dt{color:#9FAA9F}\n"
    ".al-pop dd{margin:0;font-family:\"IBM Plex Mono\",ui-monospace,Consolas,monospace;font-size:12.5px}\n"
    "@media (prefers-reduced-motion: reduce){.al *,.al-pop{transition:none!important}}\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<div id=\"arbres-lies\" class=\"al\">\n"
    "  <h3 class=\"al-titre\"></h3>\n"
    "  <div class=\"al-panels\">\n"
    "    <div class=\"al-panel\">\n"
    "      <h4>Arbre informatique</h4>\n"
    "      <p>racine (le pied) en haut</p>\n"
    "      <svg class=\"al-haut\" role=\"img\" aria-label=\"Arbre informatique, racine en haut\"></svg>\n"
    "    </div>\n"
    "    <div class=\"al-panel\">\n"
    "      <h4>Arbre informatique</h4>\n"
    "      <p>racine (le pied) en bas</p>\n"
    "      <svg class=\"al-bas\" role=\"img\" aria-label=\"Arbre informatique, racine en bas\"></svg>\n"
    "    </div>\n"
    "    <div class=\"al-panel\">\n"
    "      <h4>Arbre de rameaux</h4>\n"
    "      <p>de profil, le pied en bas</p>\n"
    "      <svg class=\"al-profil\" role=\"img\" aria-label=\"Arbre de rameaux, de profil\"></svg>\n"
    "    </div>\n"
    "  </div>\n"
    "  <div class=\"al-legende\">\n"
    "    <span><i class=\"l-bois\"></i>nœud = rameau de bois</span>\n"
    "    <span><i class=\"l-bud\"></i>nœud = rameau qui porte un bourgeon</span>\n"
    "    <span><i class=\"l-lien\"></i>lien « est né à l'extrémité de »</span>\n"
    "    <span><i class=\"l-seg\"></i>un rameau</span>\n"
    "  </div>\n"
    "  <div class=\"al-pop\" role=\"status\" aria-live=\"polite\"></div>\n"
    "</div>\n"
    "<script>\n";

/* fin du script et de la page */
static const char *html_fin =
    "(function () {\n"
    "  const racine = document.getElementById('arbres-lies');\n"
    "  const NS = 'http://www.w3.org/2000/svg';\n"
    "  const el = (tag, attrs, parent) => {\n"
    "    const e = document.createElementNS(NS, tag);\n"
    "    for (const k in attrs) e.setAttribute(k, attrs[k]);\n"
    "    if (parent) parent.appendChild(e);\n"
    "    return e;\n"
    "  };\n"
    "  const fr = (v, d = 2) => v.toFixed(d).replace('.', ',');\n"
    "  const taille = (svg, w, h) => {\n"
    "    svg.setAttribute('viewBox', `0 0 ${w} ${h}`);\n"
    "    svg.setAttribute('width', w);\n"
    "    svg.setAttribute('height', h);\n"
    "  };\n"
    "\n"
    "  // reconstitution de l'arbre : R[num] est le rameau de numéro num, ORDRE les range parent avant fils\n"
    "  const R = {}, ORDRE = [];\n"
    "  const pied = { num: PIED.num, x: PIED.x, y: PIED.y, dir: 0, lg: PIED.y, saison: 0, gen: 0, parent: -1, fils: [] };\n"
    "  R[pied.num] = pied;\n"
    "  ORDRE.push(pied);\n"
    "  for (const [num, p, saison, gen, dir, lg, x, y] of LIGNES) {\n"
    "    const r = { num, x, y, dir, lg, saison, gen, parent: p, fils: [] };\n"
    "    R[p].fils.push(num);\n"
    "    R[num] = r;\n"
    "    ORDRE.push(r);\n"
    "  }\n"
    "  // l'état est déduit des fils présents (un arbre limité en saisons a des bourgeons à sa dernière saison)\n"
    "  ORDRE.forEach(r => (r.etat = r.fils.length ? 'bois' : 'bourgeon'));\n"
    "  const H = Math.max(...ORDRE.map(r => r.gen));\n"
    "  const SMAX = Math.max(...ORDRE.map(r => r.saison));\n"
    "  racine.querySelector('.al-titre').textContent =\n"
    "    `Arbre de ${ORDRE.length} rameaux, saisons 0 à ${SMAX}` +\n"
    "    (TRONQUE ? ` (limité aux ${SAISON_MAX} premières saisons)` : '');\n"
    "\n"
    "  // disposition : feuilles côte à côte (parcours en profondeur), nœud interne au milieu de ses fils\n"
    "  let nf = 0;\n"
    "  (function place(n) {\n"
    "    const r = R[n];\n"
    "    if (!r.fils.length) { r.col = nf++; return; }\n"
    "    r.fils.forEach(place);\n"
    "    r.col = r.fils.reduce((s, f) => s + R[f].col, 0) / r.fils.length;\n"
    "  })(pied.num);\n"
    "\n"
    "  const vues = { noeuds: {}, rameaux: {} };\n"
    "\n"
    "  /* ---------- Arbres informatiques (racine en haut / en bas) ---------- */\n"
    "  const PAS_X = 46, PAS_Y = 60, MX = 30, MY = 32, AXE = 60;\n"
    "  const LI = 2 * MX + (nf - 1) * PAS_X + AXE, HI = 2 * MY + H * PAS_Y;\n"
    "  function arbreInfo(svg, enBas) {\n"
    "    taille(svg, LI, HI);\n"
    "    const tx = c => MX + c * PAS_X;\n"
    "    const ty = g => (enBas ? HI - MY - g * PAS_Y : MY + g * PAS_Y);\n"
    "    for (let g = 0; g <= H; g++) el('text', { x: LI - AXE / 2, y: ty(g) + 4, class: 'al-axis', 'text-anchor': 'middle' }, svg).textContent = `gén. ${g}`;\n"
    "    const liens = el('g', {}, svg);\n"
    "    ORDRE.forEach(r => { if (r.parent >= 0) el('line', { x1: tx(R[r.parent].col), y1: ty(R[r.parent].gen), x2: tx(r.col), y2: ty(r.gen), class: 'al-lien' }, liens); });\n"
    "    ORDRE.forEach(r => {\n"
    "      const g = el('g', { class: `al-noeud ${r.etat}`, 'data-r': r.num, tabindex: 0, role: 'button', 'aria-label': `Rameau ${r.num}` }, svg);\n"
    "      el('circle', { cx: tx(r.col), cy: ty(r.gen), r: 19 }, g);\n"
    "      el('text', { x: tx(r.col), y: ty(r.gen) }, g).textContent = r.num;\n"
    "      (vues.noeuds[r.num] ||= []).push(g);\n"
    "    });\n"
    "  }\n"
    "  arbreInfo(racine.querySelector('.al-haut'), false);\n"
    "  arbreInfo(racine.querySelector('.al-bas'), true);\n"
    "\n"
    "  /* ---------- Arbre de rameaux, de profil ---------- */\n"
    "  const sp = racine.querySelector('.al-profil');\n"
    "  let xmin = pied.x, xmax = pied.x, ymax = pied.y;\n"
    "  ORDRE.forEach(r => { xmin = Math.min(xmin, r.x); xmax = Math.max(xmax, r.x); ymax = Math.max(ymax, r.y); });\n"
    "  const XA = Math.floor(xmin), XB = Math.floor(xmax) + 1, YB = Math.floor(ymax) + 1;\n"
    "  const ECH = Math.min(95, Math.max(30, 900 / Math.max(XB - XA, YB)));   // pixels par unité de longueur\n"
    "  const L = 44, T = 22, B = T + YB * ECH;\n"
    "  taille(sp, L + (XB - XA) * ECH + 10, B + 30);\n"
    "  const px = x => L + (x - XA) * ECH;\n"
    "  const py = y => B - y * ECH;\n"
    "  const grille = el('g', { class: 'al-grid' }, sp);\n"
    "  for (let y = 0; y <= YB; y++) {\n"
    "    el('line', { x1: px(XA), x2: px(XB), y1: py(y), y2: py(y) }, grille);\n"
    "    el('text', { x: L - 8, y: py(y) + 4, class: 'al-axis', 'text-anchor': 'end' }, sp).textContent = `y = ${y}`;\n"
    "  }\n"
    "  for (let x = XA; x <= XB; x++) el('line', { x1: px(x), x2: px(x), y1: T, y2: B }, grille);\n"
    "  for (let c = XA; c < XB; c++) el('text', { x: px(c + .5), y: B + 20, class: 'al-axis', 'text-anchor': 'middle' }, sp).textContent = `colonne ${c}`;\n"
    "  el('line', { x1: px(XA), x2: px(XB), y1: B, y2: B, class: 'al-sol' }, sp);\n"
    "\n"
    "  const depart = el('circle', { r: 9, class: 'al-depart' });  // cercle sur le point de départ du rameau survolé\n"
    "  const segs = el('g', {}, sp), bouts = el('g', {}, sp);\n"
    "  ORDRE.forEach(r => {\n"
    "    const base = r.parent < 0 ? { x: r.x, y: 0 } : R[r.parent];   // le pied part du sol\n"
    "    const g = el('g', { class: `al-rameau ${r.etat}`, 'data-r': r.num }, segs);\n"
    "    el('line', { x1: px(base.x), y1: py(base.y), x2: px(r.x), y2: py(r.y), class: 'al-seg' }, g);\n"
    "    el('line', { x1: px(base.x), y1: py(base.y), x2: px(r.x), y2: py(r.y), class: 'al-zone' }, g);\n"
    "    // extrémité + numéro, dessinés au-dessus de tous les segments\n"
    "    const b = el('g', { class: `al-rameau ${r.etat}`, 'data-r': r.num }, bouts);\n"
    "    el('circle', { cx: px(r.x), cy: py(r.y), r: r.etat === 'bourgeon' ? 7 : 3.5, class: 'al-bout' }, b);\n"
    "    const dx = r.dir < -20 || (r.etat === 'bourgeon' && r.dir < 0) ? -20 : 10;\n"
    "    el('text', { x: px(r.x) + dx, y: py(r.y) + 4 }, b).textContent = r.num;\n"
    "    vues.rameaux[r.num] = [g, b];\n"
    "    r.base = base;\n"
    "  });\n"
    "  sp.appendChild(depart);\n"
    "\n"
    "  /* ---------- Survol : nœud <-> rameau, fenêtre d'information ---------- */\n"
    "  const pop = racine.querySelector('.al-pop');\n"
    "  let courant = null;\n"
    "\n"
    "  function montrer(n, ancre) {\n"
    "    if (courant === n) return;\n"
    "    cacher();\n"
    "    courant = n;\n"
    "    const r = R[n];\n"
    "    racine.classList.add('actif');\n"
    "    vues.noeuds[n].forEach(e => e.classList.add('hl'));\n"
    "    vues.rameaux[n].forEach(e => { e.classList.add('hl'); e.parentNode.appendChild(e); });\n"
    "    depart.setAttribute('cx', px(r.base.x)); depart.setAttribute('cy', py(r.base.y));\n"
    "\n"
    "    const fils = r.fils.length ? r.fils.join(', ') : 'aucun';\n"
    "    pop.innerHTML =\n"
    "      `<b>Rameau ${r.num}<em class=\"${r.etat}\">${r.etat}</em></b><dl>` +\n"
    "      `<dt>parent</dt><dd>${r.parent < 0 ? '– (c\\'est le pied)' : r.parent}</dd>` +\n"
    "      `<dt>fils</dt><dd>${fils}</dd>` +\n"
    "      `<dt>saison</dt><dd>${r.saison}</dd>` +\n"
    "      `<dt>génération</dt><dd>${r.gen}</dd>` +\n"
    "      `<dt>direction</dt><dd>${fr(r.dir, 1)}°</dd>` +\n"
    "      `<dt>longueur</dt><dd>${r.parent < 0 ? '–' : fr(r.lg, 2)}</dd>` +\n"
    "      `<dt>extrémité</dt><dd>(${fr(r.x)} ; ${fr(r.y)})</dd></dl>`;\n"
    "\n"
    "    // place la fenêtre à côté de l'élément survolé, sans sortir du bloc\n"
    "    const bc = racine.getBoundingClientRect(), ba = ancre.getBoundingClientRect();\n"
    "    pop.classList.add('on');\n"
    "    const w = pop.offsetWidth, h = pop.offsetHeight;\n"
    "    let x = ba.right - bc.left + 10, y = ba.top - bc.top + ba.height / 2 - h / 2;\n"
    "    if (x + w > bc.width - 6) x = ba.left - bc.left - w - 10;\n"
    "    x = Math.max(6, Math.min(x, bc.width - w - 6));\n"
    "    y = Math.max(6, Math.min(y, bc.height - h - 6));\n"
    "    pop.style.left = x + 'px'; pop.style.top = y + 'px';\n"
    "  }\n"
    "  function cacher() {\n"
    "    if (courant === null) return;\n"
    "    vues.noeuds[courant].forEach(e => e.classList.remove('hl'));\n"
    "    vues.rameaux[courant].forEach(e => e.classList.remove('hl'));\n"
    "    // remet les rameaux dans l'ordre de départ\n"
    "    ORDRE.forEach(r => { const [g, b] = vues.rameaux[r.num]; segs.appendChild(g); bouts.appendChild(b); });\n"
    "    racine.classList.remove('actif');\n"
    "    pop.classList.remove('on');\n"
    "    courant = null;\n"
    "  }\n"
    "\n"
    "  const cible = e => e.target.closest && e.target.closest('.al-noeud, .al-rameau');\n"
    "  racine.addEventListener('pointerover', e => {\n"
    "    const t = cible(e);\n"
    "    if (t) montrer(+t.dataset.r, t.matches('.al-rameau') ? vues.rameaux[t.dataset.r][1] : t);\n"
    "  });\n"
    "  racine.addEventListener('pointerout', e => {\n"
    "    const t = cible(e);\n"
    "    if (t && !(e.relatedTarget && e.relatedTarget.closest && e.relatedTarget.closest(`[data-r=\"${t.dataset.r}\"]`))) cacher();\n"
    "  });\n"
    "  racine.addEventListener('focusin', e => { const t = cible(e); if (t) montrer(+t.dataset.r, t); });\n"
    "  racine.addEventListener('focusout', cacher);\n"
    "})();\n"
    "</script>\n"
    "</body>\n"
    "</html>\n";

/* écrit les fils de r apparus au plus tard à la saison SAISON_MAX_HTML, puis leurs descendants,
   chacun après son parent (fonction récursive) ; renvoie 1 si des rameaux ont été omis, 0 sinon */
static int writeBranchesHtml(FILE *f, t_rameau *r)
{
    int omis = 0;
    for (int i = 0; i < r->nb_fils; i++)
    {
        t_rameau *fils = r->p_fils[i];
        if (fils->saison > SAISON_MAX_HTML)
        {
            omis = 1;
        }
        else
        {
            fprintf(f, "  [%d, %d, %d, %d, %.2f, %.2f, %.2f, %.2f],\n",
                    fils->num, r->num, fils->saison, fils->gen, fils->dir, fils->longueur, fils->x, fils->y);
            if (writeBranchesHtml(f, fils) == 1)
            {
                omis = 1;
            }
        }
    }
    return omis;
}

void drawHtml(t_rameau *pied, const char *nom_fichier)
{
    FILE *f = fopen(nom_fichier, "wt");
    if (f == NULL)
    {
        perror("Impossible de creer le fichier HTML");
        return;
    }
    fputs(html_debut, f);
    // les données de l'arbre, en JavaScript
    fprintf(f, "const PIED = { num: %d, x: %.2f, y: %.2f };\n", pied->num, pied->x, pied->y);
    fprintf(f, "const LIGNES = [\n");
    int omis = writeBranchesHtml(f, pied);
    fprintf(f, "];\n");
    fprintf(f, "const SAISON_MAX = %d;\n", SAISON_MAX_HTML);
    fprintf(f, "const TRONQUE = %s;\n", omis ? "true" : "false");
    fputs(html_fin, f);
    fclose(f);
    if (omis)
    {
        printf("%s : seules les %d premieres saisons sont dessinees\n", nom_fichier, SAISON_MAX_HTML);
    }
}
