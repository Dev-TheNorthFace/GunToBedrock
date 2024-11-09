#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class LSAT : public Item {
public:
    LSAT() : Item(lsat, ItemType::CUSTOM) {
        maxAmmo = 200;
        currentAmmo = maxAmmo;
        fireRate = 0.08f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre LSAT est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre LSAT est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre LSAT a été rechargé avec succès.");
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
        for (int i = 0; i < 4; ++i) {
            Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
            bullet.setDamage(10);
            bullet.setVelocity(player.getDirection() * 4.5f);
            player.playSound(SoundType::GUNSHOT);
        }
        player.sendMessage("Ratata. Le LSAT a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<LSAT>(lsat);
}