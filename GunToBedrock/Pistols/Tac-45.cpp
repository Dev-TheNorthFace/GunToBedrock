#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class TAC_45 : public Item {
public:
    TAC_45() : Item(tac_45, ItemType::CUSTOM) {
        maxAmmo = 7;
        currentAmmo = maxAmmo;
        fireRate = 0.7f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre TAC-45 est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre TAC-45 est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre TAC-45 a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);  
        bullet.setDamage(20);
        bullet.setVelocity(player.getDirection() * 4.5f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Tir ! Le TAC-45 a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<TAC_45>(tac_45);
}