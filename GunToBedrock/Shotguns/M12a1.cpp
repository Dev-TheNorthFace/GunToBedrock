#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class M1216 : public Item {
public:
    M1216() : Item(m1216, ItemType::CUSTOM) {
        maxAmmo = 16;
        currentAmmo = maxAmmo;
        fireRate = 1.0f;
        lastFireTime = 0.0f;
        shotsPerCycle = 4;
        shotsRemaining = 0;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre M1216 est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            player.sendMessage("Veuillez attendre avant de tirer à nouveau.");
            return;
        }

        if (shotsRemaining > 0) {
            shoot(player);
            shotsRemaining--;
            currentAmmo--;
            lastFireTime = currentTime;
        } else {
            player.sendMessage("Cycle de tir terminé, veuillez recharger.");
        }

        if (currentAmmo == 0) {
            player.sendMessage("Votre M1216 est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::SHOTGUN_SHELL)) {
            currentAmmo = maxAmmo;
            shotsRemaining = shotsPerCycle;
            player.removeItem(ItemType::SHOTGUN_SHELL, maxAmmo);
            player.sendMessage("Votre M1216 a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;
    int shotsPerCycle;
    int shotsRemaining;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(25);
        bullet.setVelocity(player.getDirection() * 2.5f);
        player.playSound(SoundType::SHOTGUN_FIRE);
        player.sendMessage("Boom. Le M1216 a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<M1216>(m1216);
}