#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class XPR50 : public Item {
public:
    XPR50() : Item(xpr_50, ItemType::CUSTOM) {
        maxAmmo = 10;
        currentAmmo = maxAmmo;
        fireRate = 2.0f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre XPR-50 est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre XPR-50 est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre XPR-50 a été rechargé avec succès.");
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
        bullet.setDamage(80);
        bullet.setVelocity(player.getDirection() * 4.5f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Bang. Le XPR-50 a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<XPR50>(xpr_50);
}