;; Geri alma (undo) iþlemleri için sýnýrlarý belirler
(setq undo-limit 20000000)  ;; Maksimum geri alma sýnýrý (20 milyon iþlem)
(setq undo-strong-limit 40000000)  ;; Daha güçlü geri alma sýnýrý (40 milyon iþlem)

(setq inhibit-startup-screen t)       ;; GNU Emacs baþlangýç ekranýný kapat
(setq initial-scratch-message nil)    ;; *scratch* buffer'ýndaki mesajý kaldýr
(setq inhibit-startup-message t)      ;; Baþlangýç mesajlarýný gizle
(setq inhibit-startup-echo-area-message t) ;; Mini-buffer'daki mesajý kapat
(setq make-backup-files nil)  ;; Yedek dosyalarýn oluþturulmasýný engelle

(menu-bar-mode -1)   ;; Üst menü çubuðunu kapat
(tool-bar-mode -1)   ;; Araç çubuðunu kapat
(scroll-bar-mode -1) ;; Kaydýrma çubuðunu kapat


(add-hook 'emacs-startup-hook
          (lambda ()
            (delete-other-windows)  ;; Önce tüm bölmeleri kapat
            (split-window-right)))  ;; Ekraný dikey olarak ikiye böl

(setq magit-diff-refine-hunk 'all)


;;Açýldýðý gibi full screen yapar
(add-to-list 'default-frame-alist '(fullscreen . fullboth)) ;; Emacs'i tam ekran aç


;; Satýr vurgulama modunu etkinleþtirir
(global-hl-line-mode 1)  ;; Mevcut satýrý vurgular
(set-face-background 'hl-line "midnight blue") ;; vurgulamalarý midnight blue yapar


;; Derleme dizininin kilidini kaldýrýr (bazý sistemlerde gereklidir)
(setq compilation-directory-locked nil)

;; Kaydýrma çubuðunu devre dýþý býrakýr (daha temiz bir görünüm saðlar)
(scroll-bar-mode -1)

;; Yerel deðiþkenleri etkinleþtirir (emacs'in dosya bazlý deðiþkenleri okumasýný saðlar)
(setq enable-local-variables t)

;; Satýr numaralarýný gösterir (her satýrýn baþýnda numara belirir)
(global-display-line-numbers-mode 1)

;; Sekme geniþliðini ayarlar (kodun daha düzenli görünmesini saðlar)
(setq-default tab-width 4)  ;; Sekme geniþliði 4 karakter olarak ayarlanýr
(setq-default indent-tabs-mode nil)  ;; Sekmeler yerine boþluk kullanýlýr

(set-frame-font "Fira Code Retina-9" nil t)


;; Parantez eþleþmelerini vurgular (hata yakalamayý kolaylaþtýrýr)
(show-paren-mode 1)  ;; Açýlan ve kapanan parantezleri vurgular

;; Kullanýþlý klavye kýsayollarý eklenir
(global-set-key (kbd "C-x C-b") 'ibuffer)  ;; Buffer listesi daha iyi bir arayüz ile gösterilir

;; Dikey bölünmüþ pencerelerde kaydýrmayý senkronize eder
(setq scroll-conservatively 101)  ;; Daha yumuþak kaydýrma saðlar
(setq mouse-wheel-scroll-amount '(1 ((shift) . 1)))  ;; Daha hassas fare tekerleði kaydýrmasý

(require 'company)  ;; Company paketini yükler
(add-hook 'after-init-hook 'global-company-mode)  ;; Emacs açýldýðýnda Company aktif olur

;; Kod yazarken otomatik tamamlama saðlar
(electric-pair-mode 1);; Otomatik parantez kapatma
(global-company-mode 1)  ;; Otomatik tamamlama desteðini açar

;; Dosyalarý otomatik olarak yeniden yükler (harici deðiþiklikleri yansýtýr)
(global-auto-revert-mode 1)  ;; Dýþarýdan deðiþtirilen dosyalarý otomatik günceller

;; Ýmleç hareketlerini daha akýcý hale getirir
(setq cursor-type 'bar)  ;; Ýmleci çubuk þeklinde gösterir
(blink-cursor-mode 0)  ;; Ýmleç yanýp sönmesini kapatýr

;; Çýkýþta onay istemesi eklenir (yanlýþlýkla çýkýþý engeller)

(setq ring-bell-function 'ignore)  ;; Emacs'teki uyarý seslerini kapatýr
