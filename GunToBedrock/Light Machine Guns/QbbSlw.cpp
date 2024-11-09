#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class QBB_LSW : public Item {
public:
    QBB_LSW() : Item(qbb_lsw, ItemType::CUSTOM) {
        maxAmmo = 150;
        currentAmmo = maxAmmo;
        fireRate = 0.07f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre QBB LSW est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre QBB LSW est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre QBB LSW a été rechargé avec succès.");
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
        for (int i = 0; i < 3; ++i) {
            Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
            bullet.setDamage(8);
            bullet.setVelocity(player.getDirection() * 4.0f);
            player.playSound(SoundType::GUNSHOT);
        }
        player.sendMessage("Ratata. Le QBB LSW a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<QBB_LSW>(qbb_lsw);
}