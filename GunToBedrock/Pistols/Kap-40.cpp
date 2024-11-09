#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class KAP40 : public Item {
public:
    KAP40() : Item("kap_40", ItemType::CUSTOM) {
        maxAmmo = 12;
        currentAmmo = maxAmmo;
        fireRate = 0.3f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre KAP-40 est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre KAP-40 est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre KAP-40 a été rechargé avec succès.");
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
        bullet.setDamage(10);
        bullet.setVelocity(player.getDirection() * 2.5f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Tir. La KAP-40 a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<KAP40>("kap_40");
}