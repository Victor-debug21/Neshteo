;; Geri alma (undo) i�lemleri i�in s�n�rlar� belirler
(setq undo-limit 20000000)  ;; Maksimum geri alma s�n�r� (20 milyon i�lem)
(setq undo-strong-limit 40000000)  ;; Daha g��l� geri alma s�n�r� (40 milyon i�lem)

(setq inhibit-startup-screen t)       ;; GNU Emacs ba�lang�� ekran�n� kapat
(setq initial-scratch-message nil)    ;; *scratch* buffer'�ndaki mesaj� kald�r
(setq inhibit-startup-message t)      ;; Ba�lang�� mesajlar�n� gizle
(setq inhibit-startup-echo-area-message t) ;; Mini-buffer'daki mesaj� kapat
(setq make-backup-files nil)  ;; Yedek dosyalar�n olu�turulmas�n� engelle

(menu-bar-mode -1)   ;; �st men� �ubu�unu kapat
(tool-bar-mode -1)   ;; Ara� �ubu�unu kapat
(scroll-bar-mode -1) ;; Kayd�rma �ubu�unu kapat


(add-hook 'emacs-startup-hook
          (lambda ()
            (delete-other-windows)  ;; �nce t�m b�lmeleri kapat
            (split-window-right)))  ;; Ekran� dikey olarak ikiye b�l

(setq magit-diff-refine-hunk 'all)


;;A��ld��� gibi full screen yapar
(add-to-list 'default-frame-alist '(fullscreen . fullboth)) ;; Emacs'i tam ekran a�


;; Sat�r vurgulama modunu etkinle�tirir
(global-hl-line-mode 1)  ;; Mevcut sat�r� vurgular
(set-face-background 'hl-line "midnight blue") ;; vurgulamalar� midnight blue yapar


;; Derleme dizininin kilidini kald�r�r (baz� sistemlerde gereklidir)
(setq compilation-directory-locked nil)

;; Kayd�rma �ubu�unu devre d��� b�rak�r (daha temiz bir g�r�n�m sa�lar)
(scroll-bar-mode -1)

;; Yerel de�i�kenleri etkinle�tirir (emacs'in dosya bazl� de�i�kenleri okumas�n� sa�lar)
(setq enable-local-variables t)

;; Sat�r numaralar�n� g�sterir (her sat�r�n ba��nda numara belirir)
(global-display-line-numbers-mode 1)

;; Sekme geni�li�ini ayarlar (kodun daha d�zenli g�r�nmesini sa�lar)
(setq-default tab-width 4)  ;; Sekme geni�li�i 4 karakter olarak ayarlan�r
(setq-default indent-tabs-mode nil)  ;; Sekmeler yerine bo�luk kullan�l�r

(set-frame-font "Fira Code Retina-9" nil t)


;; Parantez e�le�melerini vurgular (hata yakalamay� kolayla�t�r�r)
(show-paren-mode 1)  ;; A��lan ve kapanan parantezleri vurgular

;; Kullan��l� klavye k�sayollar� eklenir
(global-set-key (kbd "C-x C-b") 'ibuffer)  ;; Buffer listesi daha iyi bir aray�z ile g�sterilir

;; Dikey b�l�nm�� pencerelerde kayd�rmay� senkronize eder
(setq scroll-conservatively 101)  ;; Daha yumu�ak kayd�rma sa�lar
(setq mouse-wheel-scroll-amount '(1 ((shift) . 1)))  ;; Daha hassas fare tekerle�i kayd�rmas�

(require 'company)  ;; Company paketini y�kler
(add-hook 'after-init-hook 'global-company-mode)  ;; Emacs a��ld���nda Company aktif olur

;; Kod yazarken otomatik tamamlama sa�lar
(electric-pair-mode 1);; Otomatik parantez kapatma
(global-company-mode 1)  ;; Otomatik tamamlama deste�ini a�ar

;; Dosyalar� otomatik olarak yeniden y�kler (harici de�i�iklikleri yans�t�r)
(global-auto-revert-mode 1)  ;; D��ar�dan de�i�tirilen dosyalar� otomatik g�nceller

;; �mle� hareketlerini daha ak�c� hale getirir
(setq cursor-type 'bar)  ;; �mleci �ubuk �eklinde g�sterir
(blink-cursor-mode 0)  ;; �mle� yan�p s�nmesini kapat�r

;; ��k��ta onay istemesi eklenir (yanl��l�kla ��k��� engeller)

(setq ring-bell-function 'ignore)  ;; Emacs'teki uyar� seslerini kapat�r
